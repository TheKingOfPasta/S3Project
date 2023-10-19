# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include "network.h"
# include <time.h>

# define SEED time(NULL)

void print_help()
{
    printf("Usage: \n"
            "       --all :         all the tests\n"
            "       --network:      all the network struct relative tests\n"
            "       --feedforward:  all the feedforward method relative tests\n"
            "       --ffs:          all the feedforward_save relative tests\n"
            );

}

void test_feedforward()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    double **input = malloc(4 * sizeof(double *));
    for (size_t i = 0; i < 4; ++i)
    {
        input[i] = malloc(1 * sizeof(double));
        for (size_t j = 0; j < 1; ++j)
            input[i][j] = i;
    }
    print_matrix(input, 4, 1);
    double **output = NN_feedforward(network, input);

    printf("output: \n");
    print_matrix(output, 5, 1);
    free_matrix(output, 5);

    NN_free_network(network);
}

// ffs = feedforward_save
void test_ffs()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    double **input = malloc(4 * sizeof(double *));
    for (size_t i = 0; i < 4; ++i)
    {
        input[i] = malloc(1 * sizeof(double));
        for (size_t j = 0; j < 1; ++j)
            input[i][j] = i;
    }
    print_matrix(input, 4, 1);
    double ***zs;
    double ***output = NN_feedforward_save(network, input, &zs);

    printf("printing output: \n");
    for (size_t i = 0; i < 4; ++i)
    {
        print_matrix(output[i], network->sizes[i], 1);
    }
    printf("printing zs: \n");

    for (size_t i = 0; i < 3; ++i)
    {
        print_matrix(zs[i], network->sizes[i + 1], 1);
    }

    NN_free_network(network);

}

void test_backprop()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    double **input = malloc(4 * sizeof(double *));
    for (size_t i = 0; i < 4; ++i)
    {
        input[i] = malloc(1 * sizeof(double));
        for (size_t j = 0; j < 1; ++j)
            input[i][j] = i;
    }
    double **output = malloc(5 * sizeof(double *));
    for (size_t i = 0; i < 5; ++i)
    {
        output[i] = malloc(1 * sizeof(double));
        for (size_t j = 0; j < 1; ++j)
            output[i][j] = i;
    }
    TrainingData data = {input, output};
    backprop(network, &data);
    NN_free_network(network);
}

void test_network()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 3);
    NN_print_biases(network);
    NN_print_weights(network);

    NN_free_network(network);
}


void all()
{
    test_network();
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
    srand(SEED);


    printf("Starting test suite...\n");
    if (strcmp(argv[1], "--all") == 0)
        all();
    else if (strcmp(argv[1], "--network") == 0)
        test_network();
    else if (strcmp(argv[1], "--feedforward") == 0)
        test_feedforward();
    else if (strcmp(argv[1], "--ffs") == 0)
        test_ffs();
    else if (strcmp(argv[1], "--backprop") == 0)
        test_backprop();




    return EXIT_SUCCESS;
}

