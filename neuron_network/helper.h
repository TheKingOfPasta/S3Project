# pragma once

# include <stdlib.h>
# include <stddef.h>
# include <math.h>
# include <assert.h>
# include <string.h>


typedef struct TrainingData
{
    // An array of pixels (28 * 28) = 784
    int *image;

    // what the image represents
    int expected;
    
} TrainingData;



/**
  * The Sigmoid function used to scale down x to a value between ]-1; 1[
  * <x> is the sum of the product of the weight and input minus the bias.
  */
double sigmoid_sum(double x);

double random_value();

/**
  * Compute the sum of weight * input - bias
  * <inputs> the array coming from the previous layer
  * <n> the size of inputs and weights
  * <weights> the weights of the links to this neuron
  * <bias> the bias of the neuron
  */
double sum(const double *inputs, size_t n, const double *weights, double bias);

double sigmoid(const double *inputs, size_t n, const double *weights, double
        bias);



/**
  * Calculate vector product
  * If both a and b are 1-D arrays, it is inner product of vectors
  * (without complex conjugation).
  * Allocate a new array of size n to hold the result and returns it
  */
double *dot_1d(const double *a, const double *b, size_t n);

/**
  * Calculate matrix product
  * If A is an m x n matrix
  *    B a n * p matrix
  * then AB is a m x p matrix
  * AB is allocated in the memory
  * 1d array as 2d
  */
double *dot_2d(const double *a, size_t m, size_t n, const double *b, size_t
        p);

void swap(void *a, void *b, size_t data_size);


void shuffle(void* a, size_t n, size_t data_size);

