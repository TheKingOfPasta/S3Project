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


    return EXIT_SUCCESS;
}

