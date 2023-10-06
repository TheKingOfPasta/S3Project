# pragma once
# include "layer.h"
# include <stdlib.h>
# include "helper.h"
# include "neuron.h"

typedef struct NN_Network
{
    size_t num_layers;

    NN_Layer **layers;
} NN_Network;

void NN_free_network(NN_Network *network)
{
    for (size_t i = 0; i < network->num_layers; ++i)
        NN_free_layer(network->layers[i]);
    free(network->layers);
    free(network);
}

/**
  * Create a network from the given inputs
  * - num_layers: the number of layers the network has
  * - num_inputs: the number of inputs coming for the input layer
  * - num_neurons: an array containing the number of neurons in each layer
  *
  */
NN_Network *NN_create_network(size_t num_layers, int num_inputs, int
        *num_neurons)
{
    NN_Network *network = malloc(sizeof(NN_Network));
    assert(network != NULL);

    network->num_layers = num_layers;

    network->layers = malloc(num_layers * sizeof(NN_Layer *));
    assert(network->layers != NULL);

    // input node treated differently (0 edges before them)
    // should we omit the input layer??
    network->layers[0] = NN_create_layer(num_neurons[0], 0);
    for (size_t i = 1; i < num_layers; ++i)
    {
        // the number of input of a layer is the number of neurons from the
        // previous one
        network->layers[i] = NN_create_layer(num_neurons[i], num_neurons[i - 1]);
    }

    return network;
}

void NN_print_network(NN_Network *network)
{
    if (network == NULL)
        return; // should it crash?

    printf("Network:\n"
            "   - num_layers = %zi\n"
            "   - layers {\n", network->num_layers);

    for (size_t i = 0; i < network->num_layers; ++i)
    {
        printf("(%zi) ", i);
        NN_print_layer(network->layers[i]);
        printf("==End of Layer %zi==\n", i);
    }
    printf("}\n");
}
