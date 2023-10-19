# include "network.h"
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

/**
  * Calculate the feedforward but save result at each step
  * Return value is an array of matrices of length num_layers and dimensions
  * depending on the num of neurons of each * 1
  * Contains value after sigmoid function
  * 
  * zs is an array of matrices of length num_layers - 1 and dimensions depending
  * on the num of neurons of the next layers [1, num_layers[
  * Contains value prior to the sigmoid function
  */
double ***NN_feedforward_save(NN_Network *network, double **inputs, double
        ****zs)
{
    // inputs: matrix of size (num_neurons of previous layer, 1)
    // n x 1
    *zs = malloc((network->num_layers - 1) * sizeof(double **));
    double ***activations = malloc((network->num_layers) * sizeof(double **));
    activations[0] = inputs;
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
        add_matrix(a, b, m, 1);
        // zs holds the matrix before the sigmoid 
        (*zs)[i - 1] = a;

        // we copy a and apply sigmoid on it (copy because otherwise zs[i-1] is
        // also modified...)
        double **activation = copy_matrix(a, m, 1);
        sigmoid_matrix(activation, m);
        activations[i] = activation;

        inputs = activation;
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        printf("matrix output:\n");
        print_matrix(a, m, 1);
    }
    return activations;

}

void cost_derivative(double **output_activation, double **expected, size_t m)
{
    sub_matrix(output_activation, expected, m, 1);
}

void init_nabla(NN_Network *network, double ****nabla_b, double ****nabla_w)
{
    *nabla_b = malloc((network->num_layers - 1) * sizeof(double **));
    *nabla_w = malloc((network->num_layers - 1) * sizeof(double **));

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        (*nabla_b)[i] = init_matrix(network->sizes[i + 1], 1);
        (*nabla_w)[i] = init_matrix(network->sizes[i + 1], network->sizes[i]);
    }
}

void backprop(NN_Network *network, TrainingData *data)
{
    double ***activations, ***zs;
    double ***nabla_b, ***nabla_w;
    init_nabla(network, &nabla_b, &nabla_w);

    size_t n_lay = network->num_layers;
    size_t size = network->sizes[n_lay - 1];
    size_t prev_size = network->sizes[n_lay - 2];

    activations = NN_feedforward_save(network, data->image, &zs);


    // backward ============
    // activ[-1]
    cost_derivative(activations[n_lay - 1], data->expected, size);

    // zn[-1]
    sigmoid_prime(zs[n_lay - 2], size);
    mul_matrix(activations[n_lay - 1], zs[n_lay - 2], size, 1);
    // matrix of dim sizes[n_lay - 1]x1
    double **delta = activations[n_lay - 1];
    nabla_b[n_lay - 2] = delta;

    // now matrix of dim 1 x sizes[n_lay - 2]
    double **actiT = transpose(activations[n_lay - 2], prev_size, 1);
    // result matrix of dim sizes[n_lay - 1] x sizes[n_lay - 2]
    // nabla_w[-1]
    nabla_w[n_lay - 2] = dot_2d(delta, size, 1, actiT, prev_size);
    printf("result nabla_w: \n");
    print_matrix(nabla_w[n_lay - 2], size, prev_size);
    printf("result nabla_b:\n");
    print_matrix(nabla_b[n_lay - 2], size, 1);


    for (ssize_t i = n_lay - 2; i > 0; --i)
    {
        printf("Loop %zu/%zu\n", i, n_lay - 1);
        // TODO: structure should be good, but index are 100% wrong
        size_t next_size = network->sizes[i + 1];
        size = network->sizes[i];
        prev_size = network->sizes[i - 1];
        double **z = zs[i - 1];
        printf("z: %zu", size);
        print_matrix(z, size, 1);
        sigmoid_prime(z, size);

        // sizes[i-1] x sizes[i]
        // matrix of size (size, network->sizes[i + 1])
        double **weightT = transpose(network->weights[i], next_size, size);
        // sizes[i] x 1

        delta = dot_2d(weightT, size, next_size, delta, 1);
        mul_matrix(delta, z, size, 1);
        nabla_b[i - 1] = delta;
        printf("result nabla_b:\n");
        print_matrix(delta, size, 1);

        actiT = transpose(activations[i - 1], prev_size, 1);
        //print_matrix(actiT, 1, prev_size);
        //print_matrix(delta, size, 1);
        nabla_w[i - 1] = dot_2d(delta, size, 1, actiT, prev_size);
        printf("result nabla_w:\n");
        print_matrix(nabla_w[i - 1], size, prev_size);
    }

}
