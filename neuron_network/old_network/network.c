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

/**
  * Compute the output for each layer and keep them all
  * return a heap allocated array of array of size num_layers
  * each sub array is size num_neurons (depending on the layer)
  */
double **feedforward_all(NN_Network *network)
{
    double **activations = malloc(network->num_layers *
            sizeof(double *));
    activations[0] = data->image;
    assert(activations != NULL);
    // feedforward for all layers
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        NN_Layer *layer = network->layers[i];
        double *output = malloc(layer->num_neurons * sizeof(double));

        assert(output != NULL);
        for (size_t j = 0; j < layer->num_neurons; ++j)
        {
            output[j] =
                sigmoid_of_neuron(activations[i], layer->neurons[j],
                        network->layers[i-1]->num_neurons);
                        // TODO: add input_size in Training_Data
        }
        activations[i] = output;
    }
    return activations;
}

void free_activations(double **activations, size_t n)
{
   for (size_t i = 1; i < n; ++i)
       free(activations[i]);
   free(activations);
}

/**
  * Applies the backprop on a single layer
  *
  */
void backprop_solo(NN_Network *network, double **activations, size_t current)
{
}

void backprop(NN_Network *network, TrainingData *data)
{
    // big matrix of all layers output
    double **activations = feedforward_all(network);

    double prediction[network->layers[network->num_layers - 1]];

    // until 1 bc input layer has no weights/bias
    for (size_t i = network->num_layers - 1; i > 0; ++i)
    {
        for (size_t j = 0; j < network->layers[i]->num_neurons; ++j)
        {
            double error = (
        }
    }

    
}

// nabla =  ∇ \nabla 
void update_mini_batch(NN_Network *network, TrainingData **training, size_t k, size_t k_max, int eta)
{
    
}

/**
  * Train the neural network using mini-batch stochastic gradient descent.  
  *
  * "training_data":  is a list of TrainingData pointers
  *     - images: the input image to test
  *     - expected: the result expected from the network
  * "n": the length of "training_data"
  * "epochs": the number of epochs to train for
  * "mini_batch_size": the size of a mini batch (how much images at a time) when
  * sampling
  * "eta": the learning rate (how much the network will adapt at each step)
  */
void sdg(NN_Network *network, TrainingData **training, size_t n,  int epochs, int mini_batch_size, int eta)
{
    
    for (int i = 0; i < epochs; ++i)
    {
        shuffle((void *)training, n, sizeof(*training));

        // work with [k, k + mini_batch_size[ training data at a time
        for (size_t k = 0; k < n; k += mini_batch_size)
        {

        }
        printf("Epoch %i/%i Completed\n", i, epochs);
    }

}


