# include <stddef.h>
# include "helper.h"

typedef struct NN_Network
{
    int num_layers;

    // contains the number of neurons in the respective layers
    int *sizes;

    // A bias for each neuron in each layer in the network
    // a matrice [layer, neuron]
    double **biases;

    // A weight for each neuron in each layer in the network
    // a matrice [layer, neuron, weights]
    double ***weights;
} NN_Network;

NN_Network *NN_create_network(int *sizes, int num_layers)
{
    NN_Network *network = (NN_Network *)malloc(sizeof(NN_Network));
    
    if (network == NULL)
        errx(EXIT_FAILURE, "[NN_create_network]: Failed to allocate memory for "
                "NN_Network");

    network->num_layers = num_layers;
    network->sizes = sizes;
    network->biases = malloc(num_layers * sizeof(double));
    network->weights = malloc(num_layers * sizeof(double));

    for (int i = 0; i < num_layers; ++i)
    {
        double *layer_bias = malloc(sizes[i] * sizeof(double));        
        for (size_t j = 0; j < sizes[i]; ++j)
            layer_bias[j] = random_value();
        biases[i] = layer_bias;
    }

    // TODO: handle weights
}

void NN_free_network(NN_Network* network)
{

    for (int i = 0; i < network->num_layers; ++i)
    {
        free(network->biases[i]);
    }
    free(network->biases);
    free(network->weights);


    free(network->sizes);
    free(network);
}


/**
  * Cross the network to calculate the output at each level
  * returns the final output
  * free the inputs
  */
double *NN_feedforward(NN_Network *network, double *inputs)
{

    for (int layer = 0; layer < network->num_layers; ++layer)
    {
        double *b = network->biases[layer];
        double **w = network->weights[layer];

        int nb_neurons = network->sizes[num_layers];

        double *new_input = malloc(nb_neurons * sizeof(double));

        for (int i = 0; i < nb_neurons; ++i)
        {
            new_input[i] = sigmoid(inputs, n, w[i], b[i]);
        }
        free(inputs);
        inputs = new_input;
    }
    return inputs;


}
