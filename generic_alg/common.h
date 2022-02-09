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

    score = ((score)/(150*3))*100;
    return score;
}

    /*float score = 0;*/
    /*for (int i=0; i<150; i++){*/
        /*for (int j=0; j<3; j++)*/
            /*if (vresult_serial[i][j] == vresult_parallel[i][j])*/
                /*score++;*/
    /*}*/


    /*#pragma omp parallel for private(result) firstprivate(neural_net,test_data) num_threads(THREADS)*/
    /*double res_comp[5][3];*/
    /*#pragma omp parallel for private(result) firstprivate(neural_net,test_data)*/
    /*for(i=0; i<5; i++){*/
        /*result = (double *)malloc(sizeof(double)*3);*/
        /*result = neural_net_run_parallel(neural_net, test_data[i] + 1, 4);*/
        /*res_comp[i][0] = *result;*/
        /*res_comp[i][1] = result[1];*/
        /*res_comp[i][2] = result[2];*/
        /*[>printf("[i=%d] %lf %lf %lf -> %d\n",i, *result, result[1], result[2], classify(result, 3));<]*/
        /*free(result);*/
    /*}*/

