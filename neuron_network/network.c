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
    network->biases2 = malloc((num_layers - 1) * sizeof(Matrix *));
    network->weights2 = malloc((num_layers - 1) * sizeof(Matrix *));

    // for each layers, set its weight and biases
    for (size_t i = 1; i < num_layers; ++i)
    {
        double **layer_bias = malloc(sizes[i] * sizeof(double *));        
        // each neurons of a layers contains an array of weights
        double **layer_weights = malloc(sizes[i] * sizeof(double *));


        Matrix *bias_m = malloc(sizeof(Matrix));
        network->biases2[i - 1] = bias_m;
        Matrix *weights_m = malloc(sizeof(Matrix));
        network->weights2[i - 1] = weights_m;
        bias_m->matrix = layer_bias;
        bias_m->m = sizes[i];
        bias_m->n = 1;
        weights_m->matrix = layer_weights;
        weights_m->m = sizes[i];
        weights_m->n = sizes[i - 1];
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
    }

    return network;
}

void NN_print_biases(NN_Network *network)
{
    printf("\n--------Printing biases--------\n");
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("=====Layer %zu=====\n", i);
        print_matrix2(network->biases2[i - 1]);
    }

}

void NN_print_weights(NN_Network *network)
{
    printf("\n-------Printing weights--------\n");
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("=====Layer %zu=====\n", i);
        print_matrix2(network->weights2[i - 1]);
    }
}

void NN_free_network(NN_Network* network)
{

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        // freeing array of array of weights
        free_matrix2(network->weights2[i]);
        // freeing array of singleton biases
        free_matrix2(network->biases2[i]);
    }
    free(network->biases2);
    free(network->weights2);



    //free(network->sizes);
    free(network);
}

/**
  * Cross the network to calculate the output at each level
  * returns the final output: matrix of dimension (last_layer, 1)
  * free the inputs
  */
Matrix *NN_feedforward(NN_Network *network, Matrix *inputs)
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
        Matrix *b = network->biases2[i - 1];
        // array of size (num_neurons of previous layer, num_neurons of layer)
        Matrix *w = network->weights2[i - 1];

        printf("inputs: ");
        print_matrix2(inputs);
        printf("weights: ");
        print_matrix2(w);
        // array of size (num_neurons of layer, 1)
        Matrix *a = dot_2d2(w, inputs);
        free_matrix2(inputs);
        sigmoid_of_matrix2(a, b);
        inputs = a;
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        print_matrix2(a);
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
Matrix **NN_feedforward_save(NN_Network *network, Matrix *inputs, Matrix
        ***zs)
{
    // inputs: matrix of size (num_neurons of previous layer, 1)
    // n x 1
    *zs = malloc((network->num_layers - 1) * sizeof(Matrix *));
    Matrix **activations = malloc((network->num_layers) * sizeof(Matrix *));
    activations[0] = inputs;
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        printf("==========Layer %zu/%zu===========\n", i, network->num_layers -
                1);
        size_t m = network->sizes[i];
        size_t n = network->sizes[i - 1];

        // array of size (num_neurons of layer, 1)
        // m x n
        Matrix *b = network->biases2[i - 1];
        // array of size (num_neurons of previous layer, num_neurons of layer)
        Matrix *w = network->weights2[i - 1];

        printf("inputs: ");
        print_matrix2(inputs);
        printf("weights: ");
        print_matrix2(w);
        // array of size (num_neurons of layer, 1)
        Matrix *a = dot_2d2(w, inputs);
        add_matrix2(a, b);
        // zs holds the matrix before the sigmoid 
        (*zs)[i - 1] = a;

        // we copy a and apply sigmoid on it (copy because otherwise zs[i-1] is
        // also modified...)
        Matrix *activation = copy_matrix2(a);
        sigmoid_matrix2(activation);
        activations[i] = activation;

        inputs = activation;
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        printf("matrix output:\n");
        print_matrix2(a);
    }
    return activations;

}

void cost_derivative(Matrix *output_activation, Matrix *expected)
{
    sub_matrix2(output_activation, expected);
}

void init_nabla(NN_Network *network, Matrix ***nabla_b, Matrix ***nabla_w)
{
    *nabla_b = malloc((network->num_layers - 1) * sizeof(Matrix *));
    *nabla_w = malloc((network->num_layers - 1) * sizeof(Matrix*));

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        (*nabla_b)[i] = init_matrix2(network->sizes[i + 1], 1);
        (*nabla_w)[i] = init_matrix2(network->sizes[i + 1], network->sizes[i]);
    }
}

void backprop(NN_Network *network, TrainingData *data)
{
    Matrix **activations, **zs;
    Matrix **nabla_b, **nabla_w;
    init_nabla(network, &nabla_b, &nabla_w);

    size_t n_lay = network->num_layers;
    size_t size = network->sizes[n_lay - 1];
    size_t prev_size = network->sizes[n_lay - 2];

    activations = NN_feedforward_save(network, data->image, &zs);


    // backward ============
    // activ[-1]
    cost_derivative(activations[n_lay - 1], data->expected);

    // zn[-1]
    sigmoid_prime2(zs[n_lay - 2]);
    mul_matrix2(activations[n_lay - 1], zs[n_lay - 2]);
    // matrix of dim sizes[n_lay - 1]x1
    Matrix *delta = activations[n_lay - 1];
    nabla_b[n_lay - 2] = delta;

    // now matrix of dim 1 x sizes[n_lay - 2]
    Matrix *actiT = transpose2(activations[n_lay - 2]);
    // result matrix of dim sizes[n_lay - 1] x sizes[n_lay - 2]
    // nabla_w[-1]
    nabla_w[n_lay - 2] = dot_2d2(delta, actiT);
    printf("result nabla_w: \n");
    print_matrix2(nabla_w[n_lay - 2]);
    printf("result nabla_b:\n");
    print_matrix2(nabla_b[n_lay - 2]);


    for (ssize_t i = n_lay - 2; i > 0; --i)
    {
        printf("Loop %zu/%zu\n", i, n_lay - 1);
        // TODO: structure should be good, but index are 100% wrong
        size_t next_size = network->sizes[i + 1];
        size = network->sizes[i];
        prev_size = network->sizes[i - 1];
        Matrix *z = zs[i - 1];
        printf("z: %zu", size);
        print_matrix2(z);
        sigmoid_prime2(z);

        // sizes[i-1] x sizes[i]
        // matrix of size (size, network->sizes[i + 1])
        Matrix *weightT = transpose2(network->weights2[i]);
        // sizes[i] x 1

        delta = dot_2d2(weightT, delta);
        mul_matrix2(delta, z);
        nabla_b[i - 1] = delta;
        printf("result nabla_b:\n");
        print_matrix2(delta);

        actiT = transpose2(activations[i - 1]);
        //print_matrix(actiT, 1, prev_size);
        //print_matrix(delta, size, 1);
        nabla_w[i - 1] = dot_2d2(delta, actiT);
        printf("result nabla_w:\n");
        print_matrix2(nabla_w[i - 1]);
    }

}
