# pragma once 

# include <stdlib.h>
# include <math.h>
# include "helper.h"


/**
  * A neuron calculate its output value by doing the following calculus:
  * Let a0 to
  * a(1) = sigmoid(weight[0] * delta[0] + ... + weight[n] * delta[n])
  *
  *
  */
typedef struct NN_Neuron
{

    // Number of input connections to this neuron from the previous Layer
    // Also determine the number of weights
    size_t num_inputs;

    // Used to basculate the output
    double bias;

    // The array of weight from w_1 to w_n to determine the output value of the
    // neuron
    double *weights;

} NN_Neuron;



/**
  * Neurons and weights are created with malloc and MUST be freed before being
  * discarded
  */
void NN_free_neuron(NN_Neuron *neuron);


/**
  * Create and allocate the memory needed for a neuron
  * Then fill it with random values (weight and bias)
  * Returns a pointer to the creater neuron
  *
  * Don't forget to use NN_free_neuron(NN_Neuron *neuron) to clear your work!!!
  */
NN_Neuron* NN_create_neuron(int num_inputs);

void NN_print_neuron(NN_Neuron *neuron);

double sigmoid_of_neuron(const double *inputs, const NN_Neuron *neuron, size_t
        n);

