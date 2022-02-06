// [aignacio] - Macros for profiling code
#define START_TIME_EVAL(x)  gettimeofday(&x, NULL)
#define STOP_TIME_EVAL(x,y) gettimeofday(&y, NULL);                         \
                            Elapsed_Time = (y.tv_sec - x.tv_sec)*1000.0;    \
                            Elapsed_Time += (y.tv_usec - x.tv_usec)/1000.0

/*double *test(int tid){*/
    /*double *result = (double *)malloc(sizeof(double)*4);*/
    /*return result;*/
/*}*/

struct timeval begin, end;
double Elapsed_Time, Elapsed_Time_Serial, Elapsed_Time_Parallel;


