# include "layer.h"
# include "neuron.h"
# include <assert.h>

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

void NN_print_layer(NN_Layer *layer)
{
    if (layer == NULL) 
        return; // should crash?

    printf("Layer:\n"
            "   - num_neurons = %zi\n"
            "   - neurons {\n", layer->num_neurons);
    for (size_t i = 0; i < layer->num_neurons; ++i)
    {
        printf("(%zi) ", i);
        NN_print_neuron(layer->neurons[i]);
    }
    printf("}\n");
}
