#pragma once

#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include "../Utils/matrix.h"
#include <time.h>

typedef struct Network
{
    size_t num_layers;
    DoubleList* sizes;
    //weights and biases = list of matrix pointers
    List_m* biases;
    List_m* weights;
} Network;

Network* new_Network(size_t* sizesArr, size_t len);

void Free_Network(Network* n);

double sigmoid(double z);

Matrix* sigmoid_m(Matrix* m);

double sigmoid_prime(double z);

Matrix* sigmoid_prime_m(Matrix* m);

Matrix* feedforward(Network* n, Matrix* a);

ListTup* backprop(Network* n, Matrix* x, Matrix* y);

size_t evaluate(Network* n, Tuple_m* test_results, size_t len);

void update_mini_batch(Network* n, Tuple_m* mini_batch, size_t len, double eta);

void SGD(Network* n,
         Tuple_m* training_data,
         size_t n_training,
         size_t epochs,
         size_t mini_batch_size,
         double eta,
         Tuple_m* test_data,
         size_t n_test);