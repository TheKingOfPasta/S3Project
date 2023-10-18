# pragma once
# include <stdlib.h>
# include "helper.h"
# include "neuron.h"
# include "layer.h"

typedef struct NN_Network
{
    size_t num_layers;

    NN_Layer **layers;
} NN_Network;

void NN_free_network(NN_Network *network);

/**
  * Create a network from the given inputs
  * - num_layers: the number of layers the network has
  * - num_inputs: the number of inputs coming for the input layer
  * - num_neurons: an array containing the number of neurons in each layer
  *
  */
NN_Network *NN_create_network(size_t num_layers, int num_inputs, int
        *num_neurons);

void NN_print_network(NN_Network *network);

double *NN_feedforward(NN_Network *network, double *inputs);


