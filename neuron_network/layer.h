# include "neuron.h"
# include <stdlib.h>
# include <math.h>

typedef struct NN_Layer
{
    size_t num_neurons;

    NN_Neuron *neurons;
} NN_Layer;


void NN_free_layer(NN_Layer* layer)
{
    for (size_t i = 0; i < layer->num_neurons; ++i)
        NN_free_neuron(layer->neurons[i]);
    free(layer->neurons);
    free(layer);
}

NN_Layer *NN_create_layer(size_t num_neurons, int num_inputs)
{
    NN_Layer *layer = malloc(sizeof(NN_Layer));
    
    assert(layer != NULL);

    layer->num_neurons = num_neurons;
    layer->neurons = malloc(num_neurons * sizeof(NN_Neuron *));

    for (size_t i = 0; i < num_neurons; ++i)
        layer->neurons[i] = NN_create_neuron(num_inputs);

    return layer;
}

