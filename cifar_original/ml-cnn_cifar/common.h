#include <sys/time.h>
#include <omp.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// [aignacio] - Macros for profiling code
#define START_TIME_EVAL(x)  gettimeofday(&x, NULL)
#define STOP_TIME_EVAL(x,y) gettimeofday(&y, NULL);                         \
                            Elapsed_Time = (y.tv_sec - x.tv_sec)*1000.0;    \
                            Elapsed_Time += (y.tv_usec - x.tv_usec)/1000.0

struct timeval begin, end;
double Elapsed_Time, Elapsed_Time_Serial, Elapsed_Time_Parallel, Elapsed_Time_profiling;
int fd;
struct timeval begin2, end2;

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

void setup_gpio(){
    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    if (write(fd, "26", 2) != 2) {
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fd);
    //Set the pin to be an output by writing "out" to /sys/class/gpio/gpio26/direction

    fd = open("/sys/class/gpio/gpio26/direction", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio26/direction");
        exit(1);
    }

    if (write(fd, "out", 3) != 3) {
        perror("Error writing to /sys/class/gpio/gpio26/direction");
        exit(1);
    }

    close(fd);
}

void start_measure(){
    fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio26/value");
        exit(1);
    }

    if (write(fd, "0", 1) != 1) {
        perror("Error writing to /sys/class/gpio/gpio26/value");
        exit(1);
    }
}

void stop_measure(){
    fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio26/value");
        exit(1);
    }

    if (write(fd, "1", 1) != 1) {
        perror("Error writing to /sys/class/gpio/gpio26/value");
        exit(1);
    }
}

