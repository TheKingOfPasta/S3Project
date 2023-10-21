# pragma once
# include <stddef.h>
# include "helper.h"

typedef struct NN_Network
{
    size_t num_layers;

    // contains the number of neurons in the respective layers
    size_t *sizes;

    // A bias for each neuron in each layer in the network
    // a matrice [layer, neuron, singleton]. Singleton is a list of length 1
    // (for matrix multiplication purpose)
    Matrix **biases2;
    // A weight for each neuron in each layer in the network
    // a matrice [layer, neuron, weights]
    Matrix **weights2;
} NN_Network;

NN_Network *NN_create_network(size_t *sizes, size_t num_layers);


void NN_print_biases(NN_Network *network);
void NN_print_weights(NN_Network *network);

void NN_free_network(NN_Network* network);

//double **NN_feedforward(NN_Network *network, double **inputs);
Matrix *NN_feedforward(NN_Network *network, Matrix *inputs);


Matrix **NN_feedforward_save(NN_Network *network, Matrix *input, Matrix
        ***zs);


/*
**
  * Cross the network to calculate the output at each level
  * returns the final output
  * free the inputs
  *
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
*/
void backprop(NN_Network *network, TrainingData *data, Matrix ***in_nabla_b,
        Matrix ***in_nabla_w);
void update_mini_batch(NN_Network * network, TrainingData **data, size_t k, size_t
        k_max, double eta);
void sdg(NN_Network *network, TrainingData **data, size_t n, size_t epochs, size_t batch_size,
        double eta, TrainingData **test_data, size_t n_test);

int evaluate(NN_Network *network, TrainingData **test_data, size_t n_test);

