#include <sys/time.h>
#include <omp.h>

// [aignacio] - Macros for profiling code
#define START_TIME_EVAL(x)  gettimeofday(&x, NULL)
#define STOP_TIME_EVAL(x,y) gettimeofday(&y, NULL);                         \
                            Elapsed_Time = (y.tv_sec - x.tv_sec)*1000.0;    \
                            Elapsed_Time += (y.tv_usec - x.tv_usec)/1000.0

struct timeval begin, end;
double Elapsed_Time, Elapsed_Time_Serial, Elapsed_Time_Parallel, Elapsed_Time_profiling;

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
