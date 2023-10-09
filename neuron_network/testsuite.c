# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include "neuron.h"
# include "helper.h"
# include "layer.h"
# include "network.h"

void print_help()
{
    printf("Usage: \n"
            "       --all :         all the tests\n"
            "       --shuffle:      all the shuffle related tests\n"
            "       --neuron:       all the neuron struct relative test\n"
            "       --layer:        all the layer struct relative tests\n"
            "       --network:      all the network struct relative tests\n"
            "       --feedforward:  all the feedforward function tests\n"
            );

}

void print_list(int *a, size_t n)
{
    printf("{ ");
    for (size_t i = 0; i < n; ++i)
    {
        printf("%i ", a[i]);
    }
    printf("}\n");
}


void test_shuffle()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    print_list(a, 11);
    shuffle((void *)a, 11, sizeof(int));
    printf("After shuffle: ");
    print_list(a, 11);
}

void test_network()
{
    int neurons[] = {10, 15, 5};
    NN_Network *network = NN_create_network(3, 10, neurons);
    NN_print_network(network);

    NN_free_network(network);
}

void test_neuron()
{
    NN_Neuron *neuron = NN_create_neuron(3);
    NN_print_neuron(neuron);

    NN_free_neuron(neuron);
}

void test_layer()
{
    NN_Layer *layer = NN_create_layer(10, 13);
    NN_print_layer(layer);

    NN_free_layer(layer);
}

void test_feedforward()
{
    size_t n = 10;
    double input[10] = {0,};

    double bias = 0;
    double *weights[10] = {0,};
    
    double s = sigmoid(input, n, weights, bias);
    printf("Sigmoid is %f", s);
    

    int neurons[] = {10, 15, 5};
    NN_Network *network = NN_create_network(3, 10, neurons);
    NN_print_network(network);

    double *output = NN_feedforward(network, input);

    printf("output: { ");
    for (size_t i = 0; i < 5; ++i)
        printf("%f ", output[i]);
    printf("}");


    NN_free_network(network);

}


void all()
{
    test_shuffle();
    test_neuron();
    test_layer();
    test_network();
    test_feedforward();
}



int main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_help();
        return EXIT_SUCCESS;
    }
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "Invalid usage. Expected 1 argument");
    }


    printf("Starting test suite...\n");
    if (strcmp(argv[1], "--all") == 0)
        all();
    else if (strcmp(argv[1], "--shuffle") == 0)
        test_shuffle();
    else if (strcmp(argv[1], "--neuron") == 0)
        test_neuron();
    else if (strcmp(argv[1], "--layer") == 0)
        test_layer();
    else if (strcmp(argv[1], "--network") == 0)
        test_network();
    else if (strcmp(argv[1], "--feedforward") == 0)
        test_feedforward();

    return EXIT_SUCCESS;
}

