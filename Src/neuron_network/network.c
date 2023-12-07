# include "network.h"
# include <err.h>
# include <stdio.h>
# include <mcheck.h>
# include "cross_entropy_cost.h"
# include <pthread.h>
# include <time.h>

NN_Network *NN_create_network(size_t *sizes, size_t num_layers)
{
    NN_Network *network = (NN_Network *)malloc(sizeof(NN_Network));
    
    if (network == NULL)
        errx(EXIT_FAILURE, "[NN_create_network]: Failed to allocate memory for "
                "NN_Network");

    network->num_layers = num_layers;
    network->sizes = malloc(num_layers * sizeof(size_t));
    for (size_t i = 0; i < num_layers; ++i)
        network->sizes[i] = sizes[i];
    // num_layers - 1 because input layer has no biases nor weights
    network->biases2 = malloc((num_layers - 1) * sizeof(Matrix *));
    network->weights2 = malloc((num_layers - 1) * sizeof(Matrix *));

    // for each layers, set its weight and biases
    for (size_t i = 1; i < num_layers; ++i)
    {
        Matrix *weights_m = init_matrix2(sizes[i], sizes[i - 1]);
        Matrix *bias_m = init_matrix2(sizes[i], 1);
        
        network->biases2[i - 1] = bias_m;
        network->weights2[i - 1] = weights_m;
        for (size_t j = 0; j < sizes[i]; ++j)
        {
            bias_m->matrix[j][0] = 0;//random_value();
            for (size_t k = 0; k < sizes[i - 1]; ++k)
            {
                weights_m->matrix[j][k] = ((double)rand() / RAND_MAX / sqrt((double)sizes[i])); //random_value();
            }
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
    free(network->sizes);



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
    Matrix *b = network->biases2[0];
    Matrix *w = network->weights2[0];
    Matrix *a = dot_2d2(w, inputs);
    sigmoid_of_matrix2(a, b);
    inputs = a;
    for (size_t i = 2; i < network->num_layers; ++i)
    {
        //printf("==========Layer %zu/%zu===========\n", i, network->num_layers -
        //        1);
        // array of size (num_neurons of layer, 1)
        // m x n
        b = network->biases2[i - 1];
        // array of size (num_neurons of previous layer, num_neurons of layer)
        w = network->weights2[i - 1];

        //printf("inputs: ");
        //print_matrix2(inputs);
        //printf("weights: ");
        //print_matrix2(w);
        // array of size (num_neurons of layer, 1)
        a = dot_2d2(w, inputs);
        sigmoid_of_matrix2(a, b);
        free_matrix2(inputs);
        inputs = a;
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        //print_matrix2(a);
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
void NN_feedforward_save(NN_Network *network, Matrix *inputs, Matrix
        **zs, Matrix **activations)
{
    // inputs: matrix of size (num_neurons of previous layer, 1)
    // n x 1
    activations[0] = inputs;
    for (size_t i = 1; i < network->num_layers; ++i)
    {
        //printf("FFS() ==========Layer %zu/%zu===========\n", i, network->num_layers -
        //        1);
        // array of size (num_neurons of layer, 1)
        // m x n
        Matrix *b = network->biases2[i - 1];
        // array of size (num_neurons of previous layer, num_neurons of layer)
        Matrix *w = network->weights2[i - 1];

        // array of size (num_neurons of layer, 1)
        //Matrix *a = dot_2d2(w, inputs);
        for (size_t x = 0; x < w->m; ++x)
        {
            zs[i - 1]->matrix[x][0] = b->matrix[x][0];
            for (size_t k = 0; k < w->n; ++k)
                zs[i - 1]->matrix[x][0] += w->matrix[x][k] * inputs->matrix[k][0];
            activations[i]->matrix[x][0] = sigmoid(zs[i - 1]->matrix[x][0]);
        }

        //add_matrix2(a, b);
        // zs holds the matrix before the sigmoid 

        // we copy a and apply sigmoid on it (copy because otherwise zs[i-1] is
        // also modified...)
        //Matrix *activation = copy_matrix2(a);
        //sigmoid_matrix2(activation);
        inputs = activations[i];
        //printf("output of %zu/%zu\n", i, network->num_layers - 1);
        //printf("matrix output:\n");
        //print_matrix2(a);
    }
}

void cost_derivative(Matrix *output_activation, Matrix *expected)
{
    sub_matrix2(output_activation, expected);
}

void backprop(NN_Network *network, TrainingData *data, Matrix **nabla_b,
        Matrix **nabla_w, Matrix  **activations, Matrix **zs)
{
    Matrix *input = data->image;

    // feedforward =========
    size_t n_lay = network->num_layers;
    NN_feedforward_save(network, input, zs, activations);

    // backward ============
    // activ[-1]
    
    // compute the matrix cost_derivative * sigmoid_prime(zs[-1])
    for (size_t i = 0; i < activations[n_lay - 1]->m; ++i)
    {
        activations[n_lay - 1]->matrix[i][0] -= data->expected->matrix[i][0];
        //activations[n_lay - 1]->matrix[i][0] *= sigmoid_p(zs[n_lay - 2]->matrix[i][0]);
        nabla_b[n_lay - 2]->matrix[i][0] += activations[n_lay -
            1]->matrix[i][0];
    }
    //cost_derivative(activations[n_lay - 1], data->expected);
    //// zn[-1]
    //sigmoid_prime2(zs[n_lay - 2]);
    //mul_matrix2(activations[n_lay - 1], zs[n_lay - 2]);
    //// this zs no longer useful from here


    // matrix of dim sizes[n_lay - 1]x1
    Matrix *delta = copy_matrix2(activations[n_lay - 1]);
    //cross_entropy_delta(delta, data->expected);
    //nabla_b[n_lay - 2] = delta;

    // now matrix of dim 1 x sizes[n_lay - 2]
    //Matrix *actiT = transpose2(activations[n_lay - 2]);
    // activation no longer useful from here

    // result matrix of dim sizes[n_lay - 1] x sizes[n_lay - 2]
    // nabla_w[-1]
    for (size_t i = 0; i < delta->m; ++i)
    {
        for (size_t j = 0; j < activations[n_lay - 2]->m; ++j)
        {
            nabla_w[n_lay - 2]->matrix[i][j] += delta->matrix[i][0] * activations[n_lay -
                2]->matrix[j][0];
        }
    }

    //nabla_w[n_lay - 2] = dot_2d2(delta, actiT);
    //// actiT no longer useful
    //free_matrix2(actiT);
    //printf("result nabla_w: \n");
    //print_matrix2(nabla_w[n_lay - 2]);
    //printf("result nabla_b:\n");
    //print_matrix2(nabla_b[n_lay - 2]);


    for (ssize_t i = n_lay - 2; i > 0; --i)
    {
        //printf("Loop %zu/%zu\n", i, n_lay - 1);
        //size_t next_size = network->sizes[i + 1];
        //size = network->sizes[i];
        //prev_size = network->sizes[i - 1];
        Matrix *z = zs[i - 1];
        //sigmoid_prime2(z);

        // sizes[i-1] x sizes[i]
        // matrix of size (size, network->sizes[i + 1])
        //Matrix *weightT = transpose2(network->weights2[i]);
        // sizes[i] x 1
        Matrix *tmp_delta = init_matrix2(network->weights2[i]->n, 1);

        for (size_t x = 0; x < network->weights2[i]->n; ++x)
        {
            for (size_t k = 0; k < network->weights2[i]->m; ++k)
                tmp_delta->matrix[x][0] += network->weights2[i]->matrix[k][x]
                    * delta->matrix[k][0];
            tmp_delta->matrix[x][0] *= sigmoid_p(z->matrix[x][0]);
            nabla_b[i - 1]->matrix[x][0] += tmp_delta->matrix[x][0];
        }
        free_matrix2(delta);
        delta = tmp_delta;
        //delta = dot_2d2(weightT, delta);
        //mul_matrix2(delta, z);
        // z and weightT no longer useful from here
        //free_matrix2(weightT);
        //nabla_b[i - 1] = delta;
        //add_matrix2(nabla_b[i - 1], delta);
        //printf("result nabla_b:\n");
        //print_matrix2(delta);

        //Matrix *actiT = transpose2(activations[i - 1]);
        //print_matrix(actiT, 1, prev_size);
        //print_matrix(delta, size, 1);
        //nabla_w[i - 1] = dot_2d2(delta, actiT);
        //Matrix *dotp = dot_2d2(delta, actiT);
        for (size_t x = 0; x < delta->m; ++x)
        {
            for (size_t y = 0; y < activations[i - 1]->m; ++y)
            {
                nabla_w[i - 1]->matrix[x][y] += delta->matrix[x][0] *
                    activations[i - 1]->matrix[y][0];
            }
        }
        //add_matrix2(nabla_w[i - 1], dotp);
        //free_matrix2(dotp);
        
        // both no longer useful
        //free_matrix2(actiT);
        //printf("result nabla_w:\n");
        //print_matrix2(nabla_w[i - 1]);
    }
    free_matrix2(delta);
}

void update_weight(Matrix *weights, Matrix *nabla_w, double d_eta, double d_weights_eta)
{
    for (size_t i = 0; i < weights->m; ++i)
    {
        for (size_t j = 0; j < weights->n; ++j)
        {
            double wxw_eta = weights->matrix[i][j]; //d_weights_eta * 
            double nw_eta = nabla_w->matrix[i][j] * d_eta;
            weights->matrix[i][j] = wxw_eta - nw_eta;
            nabla_w->matrix[i][j] = 0;
        }
    }
}

void update_bias(Matrix *bias, Matrix *nabla_b, double d_eta)
{
    for (size_t i = 0; i < bias->m; ++i)
    {
        for (size_t j = 0; j < bias->n; ++j)
        {
            double b_eta = nabla_b->matrix[i][j] * d_eta;
            bias->matrix[i][j] -= b_eta;
            nabla_b->matrix[i][j] = 0;
        }
    }
}
/**
  * Update the network's weights and biases by applying gradient
  * descent using backpropagation to a single mini batch.
  * - "eta" is the  learning rate
  * - "lambda" is the regularization parameter
  * - "k" is the starting index of the data"
  * - "k_max" is the excluded end index of the data
  */
void update_mini_batch(NN_Network *network, TrainingData **data, size_t k, size_t
        k_max, double eta, double lambda, Matrix **nabla_b, Matrix **nabla_w,
        Matrix **activations, Matrix **zs)
{
    for (size_t i = k; i < k_max; ++i)
    {
        backprop(network, data[i], nabla_b, nabla_w, activations, zs);
    }

    double d_eta = eta / (k_max - k);
    double d_weights_eta = 1 - eta * (lambda / (k_max - k));
    //printf("\n\nbefore \n\n");
    //NN_print_biases(network);
    //NN_print_weights(network);

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        // could be optimised
        // compute the new weights for this layer from the delta
        update_weight(network->weights2[i], nabla_w[i], d_eta, d_weights_eta);

        update_bias(network->biases2[i], nabla_b[i], d_eta);
    }
    //printf("\nafter\n");
    //NN_print_biases(network);
}

void sdg(NN_Network *network, TrainingData **data, size_t n, size_t epochs, size_t batch_size,
        double eta, TrainingData **test_data, size_t n_test, double lambda)
{
    Matrix **nabla_b, **nabla_w;
    nabla_b = malloc((network->num_layers - 1) * sizeof(Matrix *));
    nabla_w = malloc((network->num_layers - 1) * sizeof(Matrix *));
    Matrix **zs = malloc((network->num_layers - 1) * sizeof(Matrix *));
    Matrix **activations = malloc((network->num_layers) * sizeof(Matrix *));

    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        nabla_b[i] = init_matrix2(network->sizes[i + 1], 1);
        nabla_w[i] = init_matrix2(network->sizes[i + 1], network->sizes[i]);
        activations[i + 1] = init_matrix2(network->sizes[i + 1], 1);
        zs[i] = init_matrix2(network->sizes[i + 1], 1);
    }

    for (size_t i = 0; i < epochs; ++i)
    {
        shuffle(data, n, sizeof(data[i]));

        clock_t start = clock();
        for (size_t k = 0; k < n; k += batch_size)
        {
            size_t k_max = k + batch_size;
            if (k_max > n)
                k_max = n;
            update_mini_batch(network, data, k, k_max, eta, lambda, nabla_b,
                    nabla_w, activations, zs);
        }
        double elapsed = (double)(clock() - start)/CLOCKS_PER_SEC;
        printf("Finished epoch in %fs\n", elapsed);
        if (test_data != NULL)
        {
            int positive = evaluate(network, test_data, n_test);
            printf("========Epoch %zu/%zu, %02.2f%% (%i / %zu)=========\n", i,
                    epochs - 1, ((double)positive) / n_test * 100, positive, n_test);
        }
        else
            printf("========Epoch %zu/%zu=========\n", i, epochs - 1);
    }
    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        free_matrix2(nabla_b[i]);
        free_matrix2(nabla_w[i]);
        free_matrix2(activations[i + 1]);
        free_matrix2(zs[i]);
    }
    free(zs);
    free(activations);
    free(nabla_w);
    free(nabla_b);
}

int evaluate(NN_Network *network, TrainingData **test_data, size_t n_test)
{
    int sum = 0;
    clock_t start = clock();
    for (size_t i = 0; i < n_test; ++i)
    {
        Matrix *input = test_data[i]->image;

        //print_matrix2(input);
        Matrix *output = NN_feedforward(network, input);
        int my_result = argmax(output);
        int my_expect = argmax(test_data[i]->expected);
        //printf("Expected %i but %i\n", my_expect, my_result);
        if (my_result == my_expect)
            sum++;

        free_matrix2(output);
    }
    double time_taken = (double)(clock() - start)/CLOCKS_PER_SEC;
    printf("Evaluation tine is %fs\n", time_taken);
    return sum;
}

