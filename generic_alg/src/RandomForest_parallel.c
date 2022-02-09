/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//[aignacio] Added for profiling the code
#include <sys/time.h>
#include <omp.h>
#include "common.h"
//
// node in decision tree
//
struct Node
{
    // data parameters
    int index;
    float value;

    // child nodes if the node is an internal node
    //int left;
    //int right;
    struct Node* left;
    struct Node* right;

    // node
    int left_node;
    int right_node;

    // node number
    int nNode;
    int father;

    // if the node is a leaf
    float left_leaf;
    float right_leaf;
};


float* get_class_labels(float data[][6], int rows, int cols);



//
// decision trees
//
struct Node* create_node();
struct Node** fit_model(float treeRF[][10][6], int n_estimators);
void grow(struct Node* decision_tree, float treeRF[][6], int nodeNum);

//
// predictions with the decision trees
//
float majority_vote_predict(float* predictions, int n_predictions);
float predict(struct Node *decision_tree, float* row);

void csv_dimensions(float* maxNodesTree, char fn[][100], int n_estimators);
float** create_array_2d(int n, int m);
void destroy_array_2d(float** arr);
float** read_data_2d(FILE* csv, float** data, int rows, int cols);
float*** create_array_3d(float* n, int m, int k, int total);
void destroy_array_3d(float*** arr);
float** read_data_3d(float csv[][10][6], float*** treeRF, float* maxNodesTree, int colsTree, int n_estimators, int k);
double get_accuracy(int n, float* labels, float* prediction);



float test_data[150][6] = {
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


float motherFucking3dVec[10][10][6] = {
      {
          {0.000000, 1.000000, 4.000000, 0.800000, 0.000000, 0.000000},
          {1.000000, 0.000000, 3.000000, 4.950000, 0.000000, 2.000000},
          {0.000000, 0.000000, 4.000000, 1.700000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {0.000000, 1.000000, 4.000000, 0.800000, 0.000000, 0.000000},
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {0.000000, 1.000000, 3.000000, 4.950000, 1.000000, 2.000000},
          {0.000000, 1.000000, 2.000000, 2.650000, 2.000000, 0.000000},
          {0.000000, 0.000000, 2.000000, 2.750000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {1.000000, 1.000000, 1.000000, 5.550000, 0.000000, 0.000000},
          {0.000000, 0.000000, 4.000000, 0.800000, 0.000000, 1.000000},
          {0.000000, 1.000000, 3.000000, 4.750000, 1.000000, 2.000000},
          {1.000000, 0.000000, 4.000000, 1.650000, 0.000000, 2.000000},
          {0.000000, 0.000000, 2.000000, 2.950000, 2.000000, 1.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {1.000000, 1.000000, 1.000000, 5.550000, 0.000000, 0.000000},
          {0.000000, 0.000000, 2.000000, 2.850000, 1.000000, 0.000000},
          {1.000000, 1.000000, 2.000000, 2.650000, 0.000000, 0.000000},
          {1.000000, 0.000000, 2.000000, 2.350000, 0.000000, 2.000000},
          {0.000000, 0.000000, 4.000000, 1.250000, 1.000000, 2.000000},
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {1.000000, 0.000000, 2.000000, 2.850000, 0.000000, 1.000000},
          {0.000000, 1.000000, 4.000000, 1.450000, 1.000000, 0.000000},
          {0.000000, 1.000000, 2.000000, 2.750000, 1.000000, 0.000000},
          {0.000000, 0.000000, 1.000000, 6.400000, 2.000000, 1.000000}
      },
      {
          {0.000000, 1.000000, 4.000000, 0.800000, 0.000000, 0.000000},
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {0.000000, 1.000000, 3.000000, 4.950000, 1.000000, 0.000000},
          {0.000000, 0.000000, 4.000000, 1.550000, 2.000000, 1.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {0.000000, 1.000000, 4.000000, 0.800000, 0.000000, 0.000000},
          {0.000000, 1.000000, 4.000000, 4.750000, 1.000000, 0.000000},
          {1.000000, 1.000000, 2.000000, 2.750000, 0.000000, 0.000000},
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {1.000000, 0.000000, 1.000000, 6.050000, 0.000000, 2.000000},
          {0.000000, 0.000000, 2.000000, 2.450000, 2.000000, 1.000000},
          {1.000000, 0.000000, 3.000000, 4.850000, 0.000000, 2.000000},
          {0.000000, 0.000000, 4.000000, 1.600000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {0.000000, 1.000000, 3.000000, 2.600000, 0.000000, 0.000000},
          {1.000000, 1.000000, 2.000000, 2.250000, 0.000000, 0.000000},
          {0.000000, 0.000000, 4.000000, 1.250000, 1.000000, 2.000000},
          {0.000000, 1.000000, 1.000000, 6.200000, 1.000000, 0.000000},
          {1.000000, 0.000000, 1.000000, 6.350000, 0.000000, 1.000000},
          {0.000000, 0.000000, 3.000000, 4.900000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {1.000000, 1.000000, 3.000000, 4.750000, 0.000000, 0.000000},
          {0.000000, 0.000000, 3.000000, 2.600000, 0.000000, 1.000000},
          {1.000000, 0.000000, 3.000000, 4.850000, 0.000000, 2.000000},
          {1.000000, 0.000000, 2.000000, 2.900000, 0.000000, 2.000000},
          {0.000000, 0.000000, 1.000000, 6.500000, 2.000000, 1.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {1.000000, 0.000000, 4.000000, 1.700000, 0.000000, 2.000000},
          {1.000000, 1.000000, 1.000000, 5.450000, 0.000000, 0.000000},
          {0.000000, 0.000000, 3.000000, 2.600000, 0.000000, 1.000000},
          {0.000000, 1.000000, 3.000000, 2.550000, 0.000000, 0.000000},
          {0.000000, 1.000000, 2.000000, 2.250000, 2.000000, 1.000000},
          {0.000000, 1.000000, 3.000000, 5.000000, 1.000000, 0.000000},
          {0.000000, 0.000000, 1.000000, 6.150000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      },
      {
          {0.000000, 1.000000, 3.000000, 2.600000, 0.000000, 0.000000},
          {0.000000, 1.000000, 3.000000, 4.750000, 1.000000, 0.000000},
          {1.000000, 1.000000, 2.000000, 2.750000, 0.000000, 0.000000},
          {0.000000, 0.000000, 4.000000, 1.700000, 1.000000, 2.000000},
          {0.000000, 1.000000, 1.000000, 6.850000, 2.000000, 0.000000},
          {1.000000, 0.000000, 2.000000, 3.150000, 0.000000, 2.000000},
          {0.000000, 0.000000, 4.000000, 1.800000, 1.000000, 2.000000},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
      }

  };


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  int rows = 151-1;
  int cols = 6;

  // dimensions of csv file
  int n_estimators = 10; // n_estimators = number of trees in the foreset
  //int rowsTree = 8-1;
  int colsTree = 6;
  // float* maxNodesTree = malloc(n_estimators * sizeof(float));
  float maxNodesTree[10] = {3, 5, 5, 10, 4, 8, 7, 5, 7, 7};

  // total number of rows
  int totalRows = 0;
  //  int maxMaxNodesTree = 10;
  int i;

  printf("\n[Parallel] RUN:\n");
  START_TIME_EVAL(begin);

  //#pragma omp parallel for shared(totalRows) firstprivate(maxNodesTree) num_threads(THREADS)
  for (i=0; i<n_estimators; i++) {
    //  #pragma omp critical (totalRows)
 	  totalRows = maxNodesTree[i] + totalRows;
  }
  // allocate space in memory to load all csvs
  float*** treeRF = create_array_3d(maxNodesTree,colsTree,n_estimators,totalRows);
  int k;
  // read data into memory
  for(k = 0; k < n_estimators; k++){
 	 read_data_3d(motherFucking3dVec, treeRF, maxNodesTree, colsTree, n_estimators, k);
  }

  struct Node** rf = fit_model(motherFucking3dVec, n_estimators);

  double accuracy;
  float sample[6];
  float predictions[10];
  char send_string[300];


  float result[75];
  #pragma omp parallel firstprivate(rf,test_data) num_threads(THREADS)
  for(int j = 0 ; j < 75 ; j++)
  {
    /*printf("%d ->", j);*/

    /*for(i=0; i < 4; i++){*/
      /*printf("%f, ", test_data[j][i]);*/
    /*}*/

    /*printf(" -> ");*/
    #pragma omp parallel for
  	for(i=0; i < n_estimators; i++)
  	{
  		predictions[i] = predict(rf[i], test_data[j]);
          /*printf("%.5f,", predictions[i]);*/
  	}
    #pragma omp barrier
    result[j] = majority_vote_predict(predictions, n_estimators);
    /*printf("-> %f \n", majority_vote_predict(predictions, n_estimators));*/
  }
  STOP_TIME_EVAL(begin, end);
  Elapsed_Time_Parallel = Elapsed_Time;


  for(int j = 0 ; j < 75 ; j++)
    printf("\n%d -> %f",j,result[j]);

  printf("\n[Parallel] Total execution time: %.3f ms", Elapsed_Time_Parallel);
  printf("\n");
}

//
// Dataset Cross Validation for preliminary evaluation of the random forest on the available test data
//
float* get_class_labels(float data[][6], int rows, int cols)
{
    float* class_labels = malloc(rows * sizeof(float));
    int i;
    for(i=0; i < rows; i++)
    {
        float* row = data[i];
        class_labels[i] = row[cols-1];
    }
    return class_labels;
}


//
// build up a forest of random decision trees based on the given parameters and fit to the training data
//
struct Node** fit_model(float treeRF[][10][6], int n_estimators)
{
    // father node is created here!
    struct Node** trees = (struct Node**) malloc(sizeof(struct Node) * n_estimators);
    int nodeNum = 0;
    int k;
    for(k=0; k<n_estimators; k++){
        printf("---------Loading tree %d ------------\n",k);
        struct Node* root = create_node();
        root->left_node = treeRF[k][0][0];
        root->right_node = treeRF[k][0][1];
        root->index = treeRF[k][0][2];
        root->value = treeRF[k][0][3];
        root->nNode = nodeNum;
        root->father = 1;
        grow(root, treeRF[k], nodeNum);
        trees[k] = root;
    }
    return trees;
}

//
// tree node "constructor"
//
struct Node* create_node()
{
    int* i = malloc(sizeof(int));
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->left = NULL; // true => left
    newNode->right = NULL; // false => right
    newNode->father = -1;
    *i = 1;
    newNode->right_node = -1; // end of tree in left = leaf
    newNode->left_node = -1; // end of tree in right = leaf
    newNode->right_leaf = -1; // end of tree in left = leaf
    newNode->left_leaf = -1; // end of tree in right = leaf
    newNode->nNode = -1; // end of tree in right = leaf
    newNode->index = -1; // feature_parameter_index (petal/sepal width/length) > {0-4}
    newNode->value = -1; // value corresponding to parameter_index
    return newNode;
}


//
// grow of a decision tree:
// Basicaly it start with the left node and stores the last node address.
// When the leaf node (last node) is done, the algorithm sets the flag leaf_right/leaf_left to 0;
// It starts going one node back, cheaking its leaf flags. If the flag is on, it means we should creat a left/right node, and so set it to 0.
// The algorithm ends when it reaches the root node and all nodes leaf flags are down.
// The parameters loaded to each tree is into the csv file, each line contain each node params, so the line number (and also the node number, so 'nodeNum') is stored into the tree param as nNode. So when the algorithm are cheacking the leaf node flag, the nNode param is used to acces the csv file params.
//
void grow(struct Node* decision_tree, float treeRF[][6], int nodeNum)
{
    //printf("NodeNum %d\n",nodeNum);
    if(decision_tree->left_node == 0)
    {
        //printf("leaf_left DONE! NodeNum = %d\n",decision_tree->nNode);
        decision_tree->left_leaf = treeRF[decision_tree->nNode][4];
    }
    else { // create node
        // store last node adsress
        decision_tree->left_node = 0;
        decision_tree->left = create_node();
        nodeNum++;
        decision_tree->left->nNode = nodeNum;
        decision_tree->left->left_node = treeRF[nodeNum][0];
        decision_tree->left->right_node = treeRF[nodeNum][1];
        decision_tree->left->index = treeRF[nodeNum][2];
        decision_tree->left->value = treeRF[nodeNum][3];
        //printf("I'm doing the grow_left NodeNum = %d\n",nodeNum);
        grow(decision_tree->left, treeRF, nodeNum);
    }
    if(decision_tree->right_node == 0) // create node == null
    {
        //printf("leaf_right DONE! NodeNum = %d\n",decision_tree->nNode);
        decision_tree->right_leaf = treeRF[decision_tree->nNode][5];
    }
    else { // create node
        decision_tree->right_node = 0;
        decision_tree->right = create_node();
        nodeNum++;
        decision_tree->right->nNode = nodeNum;
        decision_tree->right->left_node = treeRF[nodeNum][0];
        decision_tree->right->right_node = treeRF[nodeNum][1];
        decision_tree->right->index = treeRF[nodeNum][2];
        decision_tree->right->value = treeRF[nodeNum][3];
        //printf("I'm doing the grow_right NodeNum = %d\n",nodeNum);
        grow(decision_tree->right, treeRF, nodeNum);
    }
    if(decision_tree->left_node == 0)
    {
        //printf("leaf_left DONE! NodeNum = %d\n",decision_tree->nNode);
        decision_tree->left_leaf = treeRF[decision_tree->nNode][4];
    }
    else { // create node
        // store last node adsress
        decision_tree->left_node = 0;
        decision_tree->left = create_node();
        nodeNum++;
        decision_tree->left->nNode = nodeNum;
        decision_tree->left->left_node = treeRF[nodeNum][0];
        decision_tree->left->right_node = treeRF[nodeNum][1];
        decision_tree->left->index = treeRF[nodeNum][2];
        decision_tree->left->value = treeRF[nodeNum][3];
        //printf("I'm doing the grow_left NodeNum = %d\n",nodeNum);
        grow(decision_tree->left, treeRF, nodeNum);
    }
    if(decision_tree->right_node == 0) // create node == null
    {
        //printf("leaf_right DONE! NodeNum = %d\n",decision_tree->nNode);
        decision_tree->right_leaf = treeRF[decision_tree->nNode][5];
    }
    else { // create node
        decision_tree->right_node = 0;
        decision_tree->right = create_node();
        nodeNum++;
        decision_tree->right->nNode = nodeNum;
        decision_tree->right->left_node = treeRF[nodeNum][0];
        decision_tree->right->right_node = treeRF[nodeNum][1];
        decision_tree->right->index = treeRF[nodeNum][2];
        decision_tree->right->value = treeRF[nodeNum][3];
        //printf("I'm doing the grow_right NodeNum = %d\n",nodeNum);
        grow(decision_tree->right, treeRF, nodeNum);
    }
    if(decision_tree->left_node == 0 && decision_tree->right_node == 0){ //if this is the root and leaf left and right are done > return
        if(decision_tree->father == 1){
            //printf("DONE\n");
            return;
        }
    }
    return;
    //grow(decision_tree->addressLastNode, treeRF, nodeNum);
}

//
// predict by propagating the trained tree
//
float predict(struct Node *decision_tree, float* row)
{
    //printf("decision_tree %f\n", decision_tree->left_leaf);
    //printf("decision_tree %f\n", decision_tree->right->right_leaf);
    //printf("decision_tree %f\n", decision_tree->right->left->right_leaf);
    if(row[decision_tree->index] <= decision_tree->value)
    {
        if(decision_tree->left != NULL)
        {
            return predict(decision_tree->left, row);
        }
        else
        {
            return decision_tree->left_leaf;
        }
    } else {
        if(decision_tree->right != NULL)
        {
            return predict(decision_tree->right, row);
        }
        else
        {
            return decision_tree->right_leaf;
        }
    }
}

//
// determines the prediction based on a majority vote from an array of trees (forest)
// currently only implemented for {0,1,2} classification
//
float majority_vote_predict(float* predictions, int n_predictions)
{
    int zeroes = 0;
    int ones = 0;
    int twos = 0;
    int i;
    for(i=0; i < n_predictions; i++)
    {
        if( ((int)predictions[i]) == 0) zeroes++;
        else if( ((int)predictions[i]) == 1) ones++;
        else if( ((int)predictions[i]) == 2) twos++;
    }
    if (zeroes >= ones && zeroes >= twos){
//        if(verbose) printf("\nmajority vote: 0\n");
        return 0;
    }
    else if (ones >= zeroes && ones >= twos){
//        if(verbose) printf("\nmajority vote: 1\n");
        return 1;
    }
    else {
//        if(verbose) printf("\nmajority vote: 2\n");
        return 2;
    }
}


//
// flag for verbose output
//
int verbose = 0;

// csv file dimentions
void csv_dimensions(float* maxNodesTree, char fn[][100], int n_estimators)
{
    int rows_count = 0;
    int i, c;
    FILE *file;
    //float* row_dimention = malloc(n_estimators * sizeof(float));
    for(i=0; i<n_estimators; i++){
        rows_count = 0;
        file = fopen(fn[i],"r");
        for (c = getc(file); c != EOF; c = getc(file)){
            if (c == '\n'){// Increment count if this character is newline
                rows_count++;
            }
        }
        //printf("The file has %d lines\n ", rows_count-1);
        maxNodesTree[i] = rows_count-1;
    }
    return;
}

//
// memory management
//

// create a 2D array equivalent to a nxm matrix
//
float** create_array_2d(int n, int m)
{
    float* values = calloc(m * n, sizeof(float));
    float** rows = malloc(n * sizeof(float*));
    int i;
    for(i = 0; i < n; i++) {
        rows[i] = values + i * m;
    }
    return rows;
}

//
// free any 2D array
//
void destroy_array_2d(float** arr)
{
    free(*arr);
    free(arr);
}

//
// parse the input 2D csv file into memory
//
float** read_data_2d(FILE* csv, float** data, int rows, int cols){
    float temp;
    int i;
    int j;
    if(verbose) printf("\nloading data from csv...\n\n");
    fscanf(csv, "%*[^\n]\n"); // to read and discard the first line
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            fscanf(csv, "%f", &temp);
            data[i][j] = temp;
            fscanf(csv, ",");
            if (verbose) printf("data = %f \n", data[i][j]);
        }
        fscanf(csv,"0x0a"); // read the end of line in a .csv
        if(verbose) printf("\n");
    }
    fclose(csv);
    return 0;
}


// create a 3D array equivalent to a nxmxk matrix
//
float*** create_array_3d(float* n, int m, int k, int total)
{
    float* values = calloc(m * total, sizeof(float));
    float** nodes = malloc(total * sizeof(float*)); //rows
    float*** treeRF = malloc(k * sizeof(float**));
    int i;
    int j;
    for(i = 0; i < total; i++) {
        nodes[i] = values + i * m;
    }
    int AddressNode = 0;
    for(j = 0; j < k; j++) {
        treeRF[j] = nodes + AddressNode; // deal with it
        AddressNode = n[j] + AddressNode;
    }
    return treeRF;
}

//
// free any 3D array
//
void destroy_array_3d(float*** arr)
{
    free(**arr);
    free(*arr);
    free(arr);
}

//
// parse the input 3D csv file into memory
//
float** read_data_3d(float csv[][10][6], float*** treeRF, float* maxNodesTree, int colsTree, int n_estimators, int k){
    float temp;
    int i;
    int j;
    if(verbose) printf("\nloading data from csv...\n\n");
    // fscanf(csv, "%*[^\n]\n"); // to read and discard the first line
    for(i = 0; i < maxNodesTree[k]; i++) {
        for(j = 0; j < colsTree; j++) {
            // fscanf(csv, "%f", &temp);
            treeRF[k][i][j] = csv[k][i][j];
            //printf("treeRF = %f \n", treeRF[k][i][j]);
            // fscanf(csv, ",");
            if (verbose) printf("treeRF = %f \n", treeRF[k][i][j]);
        }
        // fscanf(csv,"0x0a"); // read the end of line in a .csv
        if(verbose) printf("\n");
    }
    //fclose(csv[k]);
    return 0;
}

//
// performance metrics calculation utilities
//
double get_accuracy(int n, float* labels, float* prediction)
{
    int correct = 0;
    int a = 0;
    int i;
    for(i=0; i < n; i++)
    {
        a = prediction[i] - labels[i];
        if(a == 0) correct++;
    }
    return (correct * 1.0 / n * 1.0) * 1.0;
}



