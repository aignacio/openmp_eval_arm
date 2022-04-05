#include <sys/time.h>
#include <omp.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

// [aignacio] - Macros for profiling code
#define START_TIME_EVAL(x)  gettimeofday(&x, NULL)
#define STOP_TIME_EVAL(x,y) gettimeofday(&y, NULL);                         \
                            Elapsed_Time = (y.tv_sec - x.tv_sec)*1000.0;    \
                            Elapsed_Time += (y.tv_usec - x.tv_usec)/1000.0

struct timeval begin, end;
double Elapsed_Time, Elapsed_Time_Serial, Elapsed_Time_Parallel, Elapsed_Time_profiling;
int fd;
struct timeval begin2, end2;

//----------------------------//
#define GPIO26 	26
#define HIGH    1
#define LOW 	0
#define INPUT   0
#define OUTPUT  1

int arquive, pin=GPIO26;
float timeSleep=0.5;
char buffer[3];
char path[35];
//----------------------------//

double get_average_proc(double *val){
    double average = 0;

    for (int i=0; i<NUM_RUNS; i++){
        average += *(val++);
    }

    return average/NUM_RUNS;
}

float compare_result(double *v1, double *v2, int iter, int cols_per_row){
    float score = 0;

    for (int i=0; i<iter; i++){
        for (int j=0; j<cols_per_row; j++){
            if (*(v1+(i*cols_per_row)+j) == *(v2+(i*cols_per_row)+j))
                score++;
            else
                printf("\n[Mismatch] -> line: %d col: %d",i,j);
        }
    }

    score = ((score)/(iter*cols_per_row))*100;
    return score;
}

float compare_result_float(float *v1, float *v2, int iter, int cols_per_row){
    float score = 0;

    for (int i=0; i<iter; i++){
        for (int j=0; j<cols_per_row; j++){
            if (*(v1+(i*cols_per_row)+j) == *(v2+(i*cols_per_row)+j))
                score++;
            else
                printf("\n[Mismatch] -> line: %d col: %d",i,j);
        }
    }

    score = ((score)/(150*3))*100;
    return score;
}

bool unexport_gpio(int pin) {
	arquive = open ("/sys/class/gpio/unexport", O_WRONLY);
    if (arquive==-1) {
        printf("Arquivo abriu incorretamente\n");
        return false;
    }
    if(write(arquive, buffer, 3) == -1) {
        close(arquive);
        return false;
    }
    return true;
}

void finalization(int nsignal) {
	// === Desvinculando o pino == //
    if(unexport_gpio(pin)){
	    delay(0.5);
        printf("\nAplicando um unexport no pino\n");
	    delay(0.5);
	    printf("Programa finalizado...\n");
	    exit(0);
    }
}

bool access_gpio(int pin) {
    snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
    if (access(path, 0) == -1) {
        return true;
    }
    else {
        return false;
    }
}

bool export_gpio(int pin) {
    arquive = open ("/sys/class/gpio/export", O_WRONLY);
    if (arquive==-1) {
        printf("Arquivo abriu incorretamente\n");
        return false;
    }
    snprintf(buffer, 3, "%d", pin);
    if(write(arquive, buffer, 3) == -1) {
        close(arquive);
        return false;
    }

    close(arquive);
    return true;
}

bool direction_gpio(int pin, int direction) {
    arquive=0;
    snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
    arquive = open (path, O_WRONLY);
    if (arquive==-1)
        return false;

    snprintf(buffer, 3, "%d", pin);
    if (write( arquive, ((direction == INPUT)?"in":"out"), 3 )==-1) {
        close(arquive);
        return false;
    }
    close(arquive);
	return true;
}

bool value_gpio(int pin, int value) {
	arquive=0;
    snprintf(path, 35, "/sys/class/gpio/gpio%d/value", pin);
    arquive = open(path, O_WRONLY);
    if (arquive == -1) {
        return false;
    }
    if (write (arquive, ((value == HIGH)?"1":"0"), 1) == -1) {
        close(arquive);
        return false;
    }
	close(arquive);
	return true;
}

void delay(float time){
	struct timespec t;
	int seg;
	seg = time;
	t.tv_sec = seg;
	t.tv_nsec = (time-seg)*1e9;
	nanosleep(&t, NULL);
}

void start_measure(){
    access_gpio(26);
    direction_gpio(26, OUTPUT);
    value_gpio(26, LOW);
}

void stop_measure(){
    value_gpio(26, HIGH);
}
