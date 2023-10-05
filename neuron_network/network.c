# include "network.h"
# include "layer.h"
# include "neuron.h"
# include "helper.h"
# include <stdlib.h>
# include <stdio.h>

/**
  * Determine the output value of the network from the given inputs
  * - Inputs is freed
  * - Allocate the output array
  */
double *NN_feedforward(NN_Network *network, double *inputs)
{
    double *output;
    for (size_t i = 0; i < network->num_layers; ++i)
    {
        NN_Layer *layer = network->layers[i];

        // output is equal to the number of neurons of this layer
        output = malloc(layer->num_neurons * sizeof(double));
        assert(output != NULL);

        // for each neuron of the layer
        for (size_t j = 0; j < layer->num_neurons; ++j)
        {
            // compute its sigmoid (depending of weights, bias and input)
            output[j] = sigmoid_of_neuron(inputs, layer->neurons[i],
                    layer->num_neurons);
        }
        // old input is now useless (taking memory)
        free(inputs);
        inputs = output; // output is the input of the next one
    }
}



void sdg(TrainingData **training, size_t n,  int epochs, int mini_batch_size, int eta)
{
    
    for (int i = 0; i < epochs; ++i)
    {
        
    }

}
