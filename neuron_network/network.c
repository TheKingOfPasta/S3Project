# include "network.h"
# include "layer.h"
# include "neuron.h"
# include "helper.h"
# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

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



/**
  * Determine the output value of the network from the given inputs
  * - Inputs is freed
  * - Allocate the output array
  */
double *NN_feedforward(NN_Network *network, double *inputs)
{
    double *previous_output = inputs;
    double *output = NULL;
    int isFirst = 1;
    int input_size = network->layers[0]->num_neurons;

    for (size_t i = 1; i < network->num_layers; ++i)
    {
        NN_Layer *layer = network->layers[i];

        // output is equal to the number of neurons of this layer
        output = malloc(layer->num_neurons * sizeof(double));
        assert(output != NULL);

        // for each neuron of the layer
        for (size_t j = 0; j < layer->num_neurons; ++j)
        {
            // compute its sigmoid (depending of weights, bias and input)
            output[j] = sigmoid_of_neuron(previous_output, layer->neurons[j],
                    input_size);
        }
        input_size = layer->num_neurons;
        // old input is now useless (taking memory)
        if (!isFirst++)
            free(previous_output);
        previous_output = output; // output is the input of the next one
    }
    return output;
}



void sdg(TrainingData **training, size_t n,  int epochs, int mini_batch_size, int eta)
{
    
    for (int i = 0; i < epochs; ++i)
    {
        
    }

}
