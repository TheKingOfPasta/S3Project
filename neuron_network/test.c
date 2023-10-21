# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include "network.h"
# include <time.h>
# include <mcheck.h>

# define SEED time(NULL)
# define ETA 0.5 // best value : 0.5
# define EPOCHS 5000 // best value : 2000
# define MINI_BATCH_SIZE 2

void print_help()
{
    printf("Usage: \n"
            "       --all :         all the tests\n"
            "       --network:      all the network struct relative tests\n"
            "       --feedforward:  all the feedforward method relative tests\n"
            "       --ffs:          all the feedforward_save relative tests\n"
            "       --umb:          all the mini_batch relative tests\n"
            );

}

void test_feedforward()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    Matrix *input = init_matrix2(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->matrix[i][j] = i;

    print_matrix2(input);
    Matrix *output = NN_feedforward(network, input);

    printf("output: \n");
    print_matrix2(output);
    free_matrix2(output);

    NN_free_network(network);
}

// ffs = feedforward_save
void test_ffs()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    Matrix *input = init_matrix2(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->matrix[i][j] = i;

    print_matrix2(input);
    Matrix **zs;
    Matrix **output = NN_feedforward_save(network, input, &zs);

    printf("printing output: \n");
    for (size_t i = 0; i < 4; ++i)
    {
        print_matrix2(output[i]);
        free_matrix2(output[i]);
    }
    free(output);

    printf("printing zs: \n");
    for (size_t i = 0; i < 3; ++i)
    {
        print_matrix2(zs[i]);
        free_matrix2(zs[i]);
    }
    free(zs);

    NN_free_network(network);

}

void test_backprop()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    Matrix *input = init_matrix2(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->matrix[i][j] = i;

    Matrix *output = init_matrix2(5, 1);
    for (size_t i = 0; i < 5; ++i)
        for (size_t j = 0; j < 1; ++j)
            output->matrix[i][j] = i;

    TrainingData data = {input, output};
    Matrix **nablab, **nablaw;
    backprop(network, &data, &nablab, &nablaw);

    printf("printing nablas\n");
    for (size_t i = 0; i < network->num_layers - 1; ++i)
    {
        printf("nabla_b:\n");
        print_matrix2(nablab[i]);
        printf("nabla_w:\n");
        print_matrix2(nablaw[i]);
        free_matrix2(nablab[i]);
        free_matrix2(nablaw[i]);
    }
    free(nablab);
    free(nablaw);
    free_matrix2(output);

    NN_free_network(network);
}

void test_network()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);
    NN_print_biases(network);
    NN_print_weights(network);

    NN_free_network(network);
}

//void create_training(TrainingData ***data, size_t n)
//{
//    *data = malloc(n * sizeof(TrainingData *));
//    for (size_t i = 0; i < n; ++i)
//    {
//
//        (*data)[i] = 
//    }
//}

void test_umb()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    Matrix *input = init_matrix2(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->matrix[i][j] = i;

    Matrix *output = init_matrix2(5, 1);
    for (size_t i = 0; i < 5; ++i)
        for (size_t j = 0; j < 1; ++j)
            output->matrix[i][j] = i;

    NN_print_weights(network);
    NN_print_biases(network);
    TrainingData data = {input, output};
    TrainingData *dataP = &data;
    update_mini_batch(network, &dataP, 0, 1, ETA);

    NN_print_weights(network);
    NN_print_biases(network);

    free_matrix2(output);
    NN_free_network(network);

}

void test_sdg()
{
    size_t neurons[] = {4, 8, 2, 5};
    NN_Network *network = NN_create_network(neurons, 4);

    Matrix *input = init_matrix2(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->matrix[i][j] = i;

    Matrix *output = init_matrix2(5, 1);
    for (size_t i = 0; i < 5; ++i)
        for (size_t j = 0; j < 1; ++j)
            output->matrix[i][j] = i;

    NN_print_weights(network);
    NN_print_biases(network);
    TrainingData data = {input, output};
    TrainingData *dataP = &data;
    
    sdg(network, &dataP, 1, 200, 10, ETA, NULL, 0);

    NN_print_weights(network);
    NN_print_biases(network);

    free_matrix2(output);
    NN_free_network(network);

}

TrainingData **xor_data()
{
    TrainingData **data = malloc(4 * sizeof(TrainingData *));
    for (size_t i = 0; i < 4; ++i)
        data[i] = malloc(sizeof(TrainingData));
    data[0]->image = init_matrix2(2, 1); // [0, 0]
    data[0]->expected = init_matrix2(2, 1); // [1, 0] = 0
    data[0]->expected->matrix[0][0] = 1;

    Matrix *a = init_matrix2(2, 1);
    a->matrix[1][0] = 1;
    data[1]->image = a; // [0, 1]
    data[1]->expected = copy_matrix2(a); // [0, 1] = 1

    a = init_matrix2(2, 1);
    a->matrix[0][0] = 1;
    data[2]->image = a;
    data[2]->expected = copy_matrix2(data[1]->expected); // [0, 1] = 1

    a = copy_matrix2(a);
    a->matrix[1][0] = 1;
    data[3]->image = a; // [1, 1]
    data[3]->expected = copy_matrix2(data[0]->expected); // [1, 0] = 0

    return data;
}

void test_xor()
{
    size_t neurons[] = {2, 2, 2};
    NN_Network *network = NN_create_network(neurons, 3);

    TrainingData **data = xor_data();

    sdg(network, data, 4, EPOCHS, MINI_BATCH_SIZE, ETA, data, 4);

    for (size_t i = 0; i < 4; ++i)
    {
        free_training_data(data[i]);
    }
    free(data);
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
    else if (strcmp(argv[1], "--umb") == 0)
        test_umb();
    else if (strcmp(argv[1], "--xor") == 0)
        test_xor();

    return EXIT_SUCCESS;
}

