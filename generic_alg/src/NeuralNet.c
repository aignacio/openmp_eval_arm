/** @file main.c
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/*
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */



/* USER CODE BEGIN (1) */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
//[aignacio] Added for profiling the code
#include <sys/time.h>
#include <omp.h>
#include "common.h"


typedef struct linked_list linked_list_t;

linked_list_t* new_linked_list();

bool linked_list_append(linked_list_t *list, void *data);

void *linked_list_remove(linked_list_t *list, int index);

bool linked_list_start_iterator(linked_list_t *list);

void *linked_list_get_next(linked_list_t *list);

int linked_list_length(linked_list_t *list);

void *linked_list_get_last(linked_list_t *list);

typedef struct neural_net neural_net_t;

neural_net_t* create_neural_net();

void neural_net_add_layer(neural_net_t *neural_net);

void neural_net_add_neuron(neural_net_t *neural_net, double *weights, int n_weights, double bias);

double * neural_net_run(neural_net_t* neural_net, double *data, int len);

double * neural_net_run_parallel(neural_net_t* neural_net, double *data, int len);


/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
int classify (double *results, int size_results){

    int final_class = 0;
    double final_score = *results;
    int i;

    for(i = 1 ; i < size_results ; i++){

        if(results[i] > final_score){
            final_class = i;
            final_score = results[i];

        }

    }

    return final_class;


}

/* USER CODE END */

int main(void)
{

//
    int i;

    neural_net_t *neural_net = create_neural_net();


    neural_net_add_layer(neural_net);
    double w_0_0[] = { 0.18284023, -0.8529724 ,  0.9295352 ,  0.27745247};
    neural_net_add_neuron(neural_net, w_0_0,  4, -0.5623624);

    double w_0_1[] = {-0.17880775, -0.42715448,  0.5908963 ,  0.45988828};
    neural_net_add_neuron(neural_net, w_0_1,  4, -0.05714933);

    double w_0_2[] = {-0.48771185, -0.3112061 , -0.3137951 ,  0.5289104 };
    neural_net_add_neuron(neural_net, w_0_2,  4, -0.053803623);

    double w_0_3[] = { 0.24778503,  0.9340235 , -1.4063437 , -1.3005494 };
    neural_net_add_neuron(neural_net, w_0_3,  4, 0.27202815);

    double w_0_4[] = { 0.72132677, -0.12656425, -1.1483048 , -0.083784  };
    neural_net_add_neuron(neural_net, w_0_4,  4, 0.41053805);

    double w_0_5[] = { 0.05532144,  0.47561175, -0.17761162, -1.3145427 };
    neural_net_add_neuron(neural_net, w_0_5,  4, 0.28362706);

    double w_0_6[] = { 0.12644108, -0.6311569 ,  0.03945397, -0.53849036};
    neural_net_add_neuron(neural_net, w_0_6,  4, 0.025350625);

    double w_0_7[] = { 0.21844819,  0.30629316, -0.961491  , -0.46816182};
    neural_net_add_neuron(neural_net, w_0_7,  4, 0.6091357);

    neural_net_add_layer(neural_net);
    double w_1_0[] = {-1.2635437 , -1.316526  ,  0.28543112,  1.3884338 ,  0.45282924,
      1.0371606 ,  0.12748991,  0.6325952 };
    neural_net_add_neuron(neural_net, w_1_0,  8, -0.43309692);

    double w_1_1[] = { 0.497385  , -0.632777  ,  0.38966534, -1.2256211 , -0.19634834,
      0.25377455,  0.6375568 , -0.5464157 };
    neural_net_add_neuron(neural_net, w_1_1,  8, -0.14760242);

    double w_1_2[] = { 0.3835234 ,  0.10146759, -0.32170305, -1.1918751 , -1.1514843 ,
     -0.8062826 , -0.69583875, -0.74578637};
    neural_net_add_neuron(neural_net, w_1_2,  8, 0.14179966);


    double test_data[150][6] = {
      {0,5.1,3.5,1.4,0.2,0.0},
      {1,4.9,3.0,1.4,0.2,0.0},
      {2,4.7,3.2,1.3,0.2,0.0},
      {3,4.6,3.1,1.5,0.2,0.0},
      {4,5.0,3.6,1.4,0.2,0.0},
      {5,5.4,3.9,1.7,0.4,0.0},
      {6,4.6,3.4,1.4,0.3,0.0},
      {7,5.0,3.4,1.5,0.2,0.0},
      {8,4.4,2.9,1.4,0.2,0.0},
      {9,4.9,3.1,1.5,0.1,0.0},
      {10,5.4,3.7,1.5,0.2,0.0},
      {11,4.8,3.4,1.6,0.2,0.0},
      {12,4.8,3.0,1.4,0.1,0.0},
      {13,4.3,3.0,1.1,0.1,0.0},
      {14,5.8,4.0,1.2,0.2,0.0},
      {15,5.7,4.4,1.5,0.4,0.0},
      {16,5.4,3.9,1.3,0.4,0.0},
      {17,5.1,3.5,1.4,0.3,0.0},
      {18,5.7,3.8,1.7,0.3,0.0},
      {19,5.1,3.8,1.5,0.3,0.0},
      {20,5.4,3.4,1.7,0.2,0.0},
      {21,5.1,3.7,1.5,0.4,0.0},
      {22,4.6,3.6,1.0,0.2,0.0},
      {23,5.1,3.3,1.7,0.5,0.0},
      {24,4.8,3.4,1.9,0.2,0.0},
      {25,5.0,3.0,1.6,0.2,0.0},
      {26,5.0,3.4,1.6,0.4,0.0},
      {27,5.2,3.5,1.5,0.2,0.0},
      {28,5.2,3.4,1.4,0.2,0.0},
      {29,4.7,3.2,1.6,0.2,0.0},
      {30,4.8,3.1,1.6,0.2,0.0},
      {31,5.4,3.4,1.5,0.4,0.0},
      {32,5.2,4.1,1.5,0.1,0.0},
      {33,5.5,4.2,1.4,0.2,0.0},
      {34,4.9,3.1,1.5,0.2,0.0},
      {35,5.0,3.2,1.2,0.2,0.0},
      {36,5.5,3.5,1.3,0.2,0.0},
      {37,4.9,3.6,1.4,0.1,0.0},
      {38,4.4,3.0,1.3,0.2,0.0},
      {39,5.1,3.4,1.5,0.2,0.0},
      {40,5.0,3.5,1.3,0.3,0.0},
      {41,4.5,2.3,1.3,0.3,0.0},
      {42,4.4,3.2,1.3,0.2,0.0},
      {43,5.0,3.5,1.6,0.6,0.0},
      {44,5.1,3.8,1.9,0.4,0.0},
      {45,4.8,3.0,1.4,0.3,0.0},
      {46,5.1,3.8,1.6,0.2,0.0},
      {47,4.6,3.2,1.4,0.2,0.0},
      {48,5.3,3.7,1.5,0.2,0.0},
      {49,5.0,3.3,1.4,0.2,0.0},
      {50,7.0,3.2,4.7,1.4,1.0},
      {51,6.4,3.2,4.5,1.5,1.0},
      {52,6.9,3.1,4.9,1.5,1.0},
      {53,5.5,2.3,4.0,1.3,1.0},
      {54,6.5,2.8,4.6,1.5,1.0},
      {55,5.7,2.8,4.5,1.3,1.0},
      {56,6.3,3.3,4.7,1.6,1.0},
      {57,4.9,2.4,3.3,1.0,1.0},
      {58,6.6,2.9,4.6,1.3,1.0},
      {59,5.2,2.7,3.9,1.4,1.0},
      {60,5.0,2.0,3.5,1.0,1.0},
      {61,5.9,3.0,4.2,1.5,1.0},
      {62,6.0,2.2,4.0,1.0,1.0},
      {63,6.1,2.9,4.7,1.4,1.0},
      {64,5.6,2.9,3.6,1.3,1.0},
      {65,6.7,3.1,4.4,1.4,1.0},
      {66,5.6,3.0,4.5,1.5,1.0},
      {67,5.8,2.7,4.1,1.0,1.0},
      {68,6.2,2.2,4.5,1.5,1.0},
      {69,5.6,2.5,3.9,1.1,1.0},
      {70,5.9,3.2,4.8,1.8,1.0},
      {71,6.1,2.8,4.0,1.3,1.0},
      {72,6.3,2.5,4.9,1.5,1.0},
      {73,6.1,2.8,4.7,1.2,1.0},
      {74,6.4,2.9,4.3,1.3,1.0},
      {75,6.6,3.0,4.4,1.4,1.0},
      {76,6.8,2.8,4.8,1.4,1.0},
      {77,6.7,3.0,5.0,1.7,1.0},
      {78,6.0,2.9,4.5,1.5,1.0},
      {79,5.7,2.6,3.5,1.0,1.0},
      {80,5.5,2.4,3.8,1.1,1.0},
      {81,5.5,2.4,3.7,1.0,1.0},
      {82,5.8,2.7,3.9,1.2,1.0},
      {83,6.0,2.7,5.1,1.6,1.0},
      {84,5.4,3.0,4.5,1.5,1.0},
      {85,6.0,3.4,4.5,1.6,1.0},
      {86,6.7,3.1,4.7,1.5,1.0},
      {87,6.3,2.3,4.4,1.3,1.0},
      {88,5.6,3.0,4.1,1.3,1.0},
      {89,5.5,2.5,4.0,1.3,1.0},
      {90,5.5,2.6,4.4,1.2,1.0},
      {91,6.1,3.0,4.6,1.4,1.0},
      {92,5.8,2.6,4.0,1.2,1.0},
      {93,5.0,2.3,3.3,1.0,1.0},
      {94,5.6,2.7,4.2,1.3,1.0},
      {95,5.7,3.0,4.2,1.2,1.0},
      {96,5.7,2.9,4.2,1.3,1.0},
      {97,6.2,2.9,4.3,1.3,1.0},
      {98,5.1,2.5,3.0,1.1,1.0},
      {99,5.7,2.8,4.1,1.3,1.0},
      {100,6.3,3.3,6.0,2.5,2.0},
      {101,5.8,2.7,5.1,1.9,2.0},
      {102,7.1,3.0,5.9,2.1,2.0},
      {103,6.3,2.9,5.6,1.8,2.0},
      {104,6.5,3.0,5.8,2.2,2.0},
      {105,7.6,3.0,6.6,2.1,2.0},
      {106,4.9,2.5,4.5,1.7,2.0},
      {107,7.3,2.9,6.3,1.8,2.0},
      {108,6.7,2.5,5.8,1.8,2.0},
      {109,7.2,3.6,6.1,2.5,2.0},
      {110,6.5,3.2,5.1,2.0,2.0},
      {111,6.4,2.7,5.3,1.9,2.0},
      {112,6.8,3.0,5.5,2.1,2.0},
      {113,5.7,2.5,5.0,2.0,2.0},
      {114,5.8,2.8,5.1,2.4,2.0},
      {115,6.4,3.2,5.3,2.3,2.0},
      {116,6.5,3.0,5.5,1.8,2.0},
      {117,7.7,3.8,6.7,2.2,2.0},
      {118,7.7,2.6,6.9,2.3,2.0},
      {119,6.0,2.2,5.0,1.5,2.0},
      {120,6.9,3.2,5.7,2.3,2.0},
      {121,5.6,2.8,4.9,2.0,2.0},
      {122,7.7,2.8,6.7,2.0,2.0},
      {123,6.3,2.7,4.9,1.8,2.0},
      {124,6.7,3.3,5.7,2.1,2.0},
      {125,7.2,3.2,6.0,1.8,2.0},
      {126,6.2,2.8,4.8,1.8,2.0},
      {127,6.1,3.0,4.9,1.8,2.0},
      {128,6.4,2.8,5.6,2.1,2.0},
      {129,7.2,3.0,5.8,1.6,2.0},
      {130,7.4,2.8,6.1,1.9,2.0},
      {131,7.9,3.8,6.4,2.0,2.0},
      {132,6.4,2.8,5.6,2.2,2.0},
      {133,6.3,2.8,5.1,1.5,2.0},
      {134,6.1,2.6,5.6,1.4,2.0},
      {135,7.7,3.0,6.1,2.3,2.0},
      {136,6.3,3.4,5.6,2.4,2.0},
      {137,6.4,3.1,5.5,1.8,2.0},
      {138,6.0,3.0,4.8,1.8,2.0},
      {139,6.9,3.1,5.4,2.1,2.0},
      {140,6.7,3.1,5.6,2.4,2.0},
      {141,6.9,3.1,5.1,2.3,2.0},
      {142,5.8,2.7,5.1,1.9,2.0},
      {143,6.8,3.2,5.9,2.3,2.0},
      {144,6.7,3.3,5.7,2.5,2.0},
      {145,6.7,3.0,5.2,2.3,2.0},
      {146,6.3,2.5,5.0,1.9,2.0},
      {147,6.5,3.0,5.2,2.0,2.0},
      {148,6.2,3.4,5.4,2.3,2.0},
      {149,5.9,3.0,5.1,1.8,2.0}
  };

    double *result;
    double vresult_serial[150][3];
    double vresult_parallel[150][3];

    printf("\n[Serial] RUN:\n");
    START_TIME_EVAL(begin);
    for(i = 0 ; i < 150 ; i++){
        result = neural_net_run(neural_net, test_data[i] + 1, 4);
        printf("%lf %lf %lf -> %d\n", *result, result[1], result[2], classify(result, 3));
        for (int j=0;j<3;j++)
            vresult_serial[i][j] = *(result+j);
        free(result);
    }
    STOP_TIME_EVAL(begin, end);
    Elapsed_Time_Serial = Elapsed_Time;


    printf("\n[Parallel] RUN:\n");
    omp_set_num_threads(THREADS);
    START_TIME_EVAL(begin);

    #pragma omp parallel for private(result) schedule(static) firstprivate(neural_net,test_data)
    for(i = 0 ; i < 150 ; i++){
        result = (double *)malloc(sizeof(double)*4);
        result = neural_net_run_parallel(neural_net, test_data[i] + 1, 4);
        printf("[th_id=%d][%d] %lf %lf %lf -> %d\n", omp_get_thread_num(),
                                                     i,
                                                     *result,
                                                     result[1],
                                                     result[2],
                                                     classify(result, 3));
        for (int j=0;j<3;j++)
            vresult_parallel[i][j] = *(result+j);
        free(result);
    }
    STOP_TIME_EVAL(begin, end);
    Elapsed_Time_Parallel = Elapsed_Time;

    /*// Scoreboard*/
    float score = compare_result(vresult_serial, vresult_parallel, 150, 3);
    printf("\n[Serial] Total execution time: %.3f ms", Elapsed_Time_Serial);
    printf("\n[Parallel] Total execution time: %.3f ms", Elapsed_Time_Parallel);
    printf("\n[Score] %.2f%% ---> %.2f%% of parallel results are wrong!", score, 100.0-score);
    printf("\n");
    return 0;
}

struct neural_net{

    linked_list_t *layers;

};

typedef struct{

    linked_list_t* neurons;
//  double bias;


}layer_t;


typedef struct{

    double* weights;
    double bias;
    int n_weights;


}neuron_t;

neural_net_t* create_neural_net(){

    neural_net_t *neural_net = (neural_net_t*)malloc(sizeof(neural_net_t));
    neural_net->layers = new_linked_list();

    return neural_net;

}

void neural_net_add_layer(neural_net_t *neural_net){

    layer_t *layer = malloc(sizeof(layer_t));
    layer->neurons = new_linked_list();
    linked_list_append(neural_net->layers, layer);

}

void neural_net_add_neuron(neural_net_t *neural_net, double *weights, int n_weights, double bias){

    layer_t *last_layer = linked_list_get_last(neural_net->layers);

    neuron_t *neuron  = (neuron_t*)malloc(sizeof(neuron_t));
    neuron->weights = weights;
    neuron->n_weights = n_weights;
    neuron->bias = bias;

    linked_list_append(last_layer->neurons, neuron);

}


double _activation_func(double val){
    return 1/(1+exp(-val));
}

double _neuron_evaluate(neuron_t* neuron, double *data){

    int i;
    double result;
    for(i = 0, result = 0 ; i <  neuron->n_weights ; i++){

        result += neuron->weights[i] * data[i];

    }


    return _activation_func(result + neuron->bias);
}

double _neuron_evaluate_parallel(neuron_t* neuron, double *data){

    int i;
    double result;

    result = 0;
    // [aignacio] We can launch the parallelization here with the following clauses
    // as firstprivate for the neuron/data once they're not zeroed and shared for the
    // common variable result, also adding the critical section for it of course.
    //
    // private = Specifies that each thread should have its own instance of a variable.
    // firstprivate	= Specifies that each thread should have its own instance of a variable,
    //                  and that the variable should be initialized with the value of the variable,
    //                  because it exists before the parallel construct.
    /*#pragma omp parallel for firstprivate(neuron,data) //reduction(+:result)*/
    /*#pragma omp parallel for firstprivate(neuron,data) //reduction(+:result)*/
    for(i=0; i<neuron->n_weights; i++){
        /*#pragma omp critical (result)*/
        result += neuron->weights[i] * data[i];
    }
    return _activation_func(result + neuron->bias);
}

double * neural_net_run_parallel(neural_net_t* neural_net, double *data, int len){

    layer_t *layer;
    neuron_t *neuron;
    int i;
    double *prev_outputs = (double *)malloc(sizeof(double)*len);
    double *next_outputs = NULL;

    linked_list_start_iterator(neural_net->layers);


    memcpy(prev_outputs, data, sizeof(double)*len);

    // [aignacio] This while loop doesn't worth to run in parallel due to the
    // fact that the layer 1 relies on the outputs of layer 0, thus adding
    // the openmp will only lead to overhead of thread mgmt like copying
    // private data to the TCBs
    while(layer = linked_list_get_next(neural_net->layers)){

        linked_list_start_iterator(layer->neurons);
        next_outputs = malloc(sizeof(double) * (linked_list_length(layer->neurons)));
        i = 0;

        // [aignacio] Same here...
        while(neuron = linked_list_get_next(layer->neurons)){
            // [aignacio] Here it might worth to parallelize the run (within _neuron_evaluate)
            next_outputs[i] = _neuron_evaluate_parallel(neuron, prev_outputs);
            i++;

        }

        free(prev_outputs);
        prev_outputs = next_outputs;


    }

    return prev_outputs;
}

double * neural_net_run(neural_net_t* neural_net, double *data, int len){

    layer_t *layer;
    neuron_t *neuron;
    int i;
    double result;
    double *prev_outputs = (double *)malloc(sizeof(double)*len);
    double *next_outputs = NULL;

    linked_list_start_iterator(neural_net->layers);


    memcpy(prev_outputs, data, sizeof(double)*len);

    while(layer = linked_list_get_next(neural_net->layers)){

        linked_list_start_iterator(layer->neurons);
        result = 0;
        next_outputs = malloc(sizeof(double) * (linked_list_length(layer->neurons)));
//      next_outputs[linked_list_length(layer->neurons)] = layer->bias;
        i = 0;

        while(neuron = linked_list_get_next(layer->neurons)){

            next_outputs[i] = _neuron_evaluate(neuron, prev_outputs);
            i++;

        }

        free(prev_outputs);
        prev_outputs = next_outputs;


    }

    return prev_outputs;
}



typedef struct node node_t;

struct node{

    void *data;
    node_t *next;

};


struct linked_list{

    node_t *head;
    node_t *tail;
    node_t *iterator;
    int length;

};


linked_list_t* new_linked_list(){

    linked_list_t *linked_list = (linked_list_t*)malloc(sizeof(linked_list_t));

    linked_list->head = NULL;
    linked_list->iterator = NULL;
    linked_list->tail = NULL;
    linked_list->length = 0;

    return linked_list;

}

bool linked_list_append(linked_list_t *list, void *data){

    node_t *node = (node_t*) malloc(sizeof(node_t));

    node->data = data;
    node->next = NULL;

    if(list->head == NULL){
        list->head = node;
        list->tail = node;
    }else{
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;

}

void *linked_list_remove(linked_list_t *list, int index){


    if(index >= list->length){
        return NULL;
    }

    if(index == 0){

        void *data = list->head->data;
        node_t *node_to_remove = list->head;
        list->head = list->head->next;

        free(node_to_remove);

        if(list->head == NULL){
            list->tail == NULL;
        }

        list->length--;
        return data;

    }


    int current_index = 0;
    node_t* node = list->head;

    while(current_index < index - 1){
        current_index++;
        node = node->next;
    }

    node_t* node_to_remove = node->next;
    node->next = node_to_remove->next;
    void *data = node_to_remove->data;

    if(index == list->length - 1){
        list->tail = node;
    }

    free(node_to_remove);
    list->length--;

    return data;


}

bool linked_list_start_iterator(linked_list_t *list){
    list->iterator = list->head;
}

void *linked_list_get_next(linked_list_t *list){

    if(list->iterator == NULL){
        return NULL;
    }

    void *data = list->iterator->data;
    list->iterator = list->iterator->next;
    return data;

}

int linked_list_length(linked_list_t *list){

    return list->length;

}


void *linked_list_get_last(linked_list_t *list){

    return list->tail->data;

}


