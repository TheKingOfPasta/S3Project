# include <malloc.h>

typedef struct NN_Network
{
    int num_layers;

    // contains the number of neurons in the respective layers
    int *sizes;

    // A bias for each neuron in each layer in the network
    // a matrice [layer, neuron]
    double **biases;

    // A weight for each neuron in each layer in the network
    // a matrice [layer, neuron]
    double **weights;
} NN_Network;

NN_Network *NN_create_network(int *sizes)
{
    NN_Network *network = (NN_Network *)malloc(sizeof(NN_Network));
    
    if (network == NULL)
        errx(EXIT_FAILURE, "[NN_create_network]: Failed to allocate memory for "
                "NN_Network");

    network->sizes = sizes;
}

void NN_free_network(NN_Network* network)
{
    free(network->sizes);
    free(network->biases);
    free(network->weights);
    free(network);
}
