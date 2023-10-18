# include "network_old.h"
# include <err.h>
# include <stdio.h>

NN_Network *NN_create_network(size_t *sizes, size_t num_layers)
{
    NN_Network *network = (NN_Network *)malloc(sizeof(NN_Network));
    
    if (network == NULL)
        errx(EXIT_FAILURE, "[NN_create_network]: Failed to allocate memory for "
                "NN_Network");

    network->num_layers = num_layers;
    network->sizes = sizes;
    // num_layers - 1 because input layer has no biases nor weights
    network->biases = malloc((num_layers - 1) * sizeof(double *));
    network->weights = malloc((num_layers - 1) * sizeof(double ***));

    // for each layers, set its weight and biases
    for (size_t i = 1; i < num_layers; ++i)
    {
        double **layer_bias = malloc(sizes[i] * sizeof(double *));        
        // each neurons of a layers contains an array of weights
        double **layer_weights = malloc(sizes[i] * sizeof(double *));
        for (size_t j = 0; j < sizes[i]; ++j)
        {
            double *bias = malloc(sizeof(double));
            *bias = random_value();
            layer_bias[j] = bias;
            double *weights = malloc(sizes[i - 1] * sizeof(double));
            for (size_t k = 0; k < sizes[i - 1]; ++k)
            {
                weights[k] = random_value();
            }
            layer_weights[j] = weights;
        }
        network->weights[i - 1] = layer_weights;
        network->biases[i - 1] = layer_bias;
    }

    return network;

    // TODO: handle weights
}

void NN_print_biases(NN_Network *network)
{
    printf("\n--------Printing biases--------\n");
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("=====Layer %zu=====\n", i);
        for (size_t j = 0; j < network->sizes[i]; ++j)
        {
            printf("- Neuron %zu: %f\n", j, network->biases[i - 1][j][0]);
        }
    }

}

void NN_print_weights(NN_Network *network)
{
    printf("\n-------Printing weights--------\n");
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("=====Layer %zu=====\n", i);
        for (size_t j = 0; j < network->sizes[i]; ++j)
        {
            printf("- Neuron %zu: {", j);
            for (size_t k = 0; k < network->sizes[i - 1]; ++k)
            {
                printf("%f, ", network->weights[i - 1][j][k]);
            }
            printf("}\n");
        }
    }
}

void NN_free_network(NN_Network* network)
{

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        for (size_t j = 0; j < network->sizes[i + 1]; ++j)
        {
            // freeing singleton bias
            free(network->biases[i][j]);
            // freeing array of weights
            free(network->weights[i][j]);
        }
        // freeing array of array of weights
        free(network->weights[i]);
        // freeing array of singleton biases
        free(network->biases[i]);
    }
    free(network->biases);
    free(network->weights);



    //free(network->sizes);
    free(network);
}

/**
  * Cross the network to calculate the output at each level
  * returns the final output: matrix of dimension (last_layer, 1)
  * free the inputs
  */
double **NN_feedforward(NN_Network *network, double **inputs)
{
    // inputs: matrix of size (num_neurons of previous layer, 1)
    // n x 1
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("==========Layer %zu/%zu===========\n", i, network->num_layers -
                1);
        size_t m = network->sizes[i];
        size_t n = network->sizes[i - 1];

        // array of size (num_neurons of layer, 1)
        // m x n
        double **b = network->biases[i - 1];
        // array of size (num_neurons of previous layer, num_neurons of layer)
        double **w = network->weights[i - 1];

        printf("inputs: ");
        print_matrix(inputs, n, 1);
        printf("weights: ");
        print_matrix(w, m, n);
        // array of size (num_neurons of layer, 1)
        double **a = dot_2d(w, m, n, inputs, 1);
        free_matrix(inputs, n);
        sigmoid_of_matrix(a, b, m);
        inputs = a;
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        print_matrix(a, m, 1);
    }
    return inputs;
}
