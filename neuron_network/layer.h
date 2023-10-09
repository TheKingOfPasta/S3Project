# pragma once
# include "neuron.h"
# include <stdlib.h>
# include <math.h>

typedef struct NN_Layer
{
    size_t num_neurons;

    // array of pointers of neurons
    NN_Neuron **neurons;
} NN_Layer;


void NN_free_layer(NN_Layer* layer);


NN_Layer *NN_create_layer(size_t num_neurons, int num_inputs);

void NN_print_layer(NN_Layer *layer);

