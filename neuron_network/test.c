# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include <time.h>
# include <mcheck.h>

# include "network.h"
# include "mnist_reader.h"
# include "network_loader.h"

# define SEED time(NULL)

// for the XOR function
# define XOR_ETA 0.5 // best value : 0.5
# define XOR_EPOCHS 5000 // best value : 2000
# define XOR_MINI_BATCH_SIZE 2
# define XOR_LAMBDA 5.0

// for the digits function
# define DIGITS_ETA 10
# define DIGITS_EPOCHS 30
# define DIGITS_MINI_BATCH_SIZE 10
# define DIGITS_LAMBDA 5.0

void print_help()
{
    printf("Usage: \n"
            "       --all :         all the tests\n"
            "       --network:      all the network struct relative tests\n"
            "       --feedforward:  all the feedforward method relative tests\n"
            "       --ffs:          all the feedforward_save relative tests\n"
            "       --umb:          all the mini_batch relative tests\n"
            "       --xor:          try to teach the network the XOR function\n"
            "       --mnist:        try to import the mnist dataset\n"
            "       --digits:       try to teach the mnist dataset\n"
            "       --save [path]:         try to save a network\n"
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
    //size_t neurons[] = {4, 8, 2, 5};
    //NN_Network *network = NN_create_network(neurons, 4);

    //Matrix *input = init_matrix2(4, 1);

    //for (size_t i = 0; i < 4; ++i)
    //    for (size_t j = 0; j < 1; ++j)
    //        input->matrix[i][j] = i;

    //print_matrix2(input);
    //Matrix **zs;
    //Matrix **output = NN_feedforward_save(network, input, &zs);

    //printf("printing output: \n");
    //for (size_t i = 0; i < 4; ++i)
    //{
    //    print_matrix2(output[i]);
    //    free_matrix2(output[i]);
    //}
    //free(output);

    //printf("printing zs: \n");
    //for (size_t i = 0; i < 3; ++i)
    //{
    //    print_matrix2(zs[i]);
    //    free_matrix2(zs[i]);
    //}
    //free(zs);

    //NN_free_network(network);

}

void test_backprop()
{
    //size_t neurons[] = {4, 8, 2, 5};
    //NN_Network *network = NN_create_network(neurons, 4);

    //Matrix *input = init_matrix2(4, 1);

    //for (size_t i = 0; i < 4; ++i)
    //    for (size_t j = 0; j < 1; ++j)
    //        input->matrix[i][j] = i;

    //Matrix *output = init_matrix2(5, 1);
    //for (size_t i = 0; i < 5; ++i)
    //    for (size_t j = 0; j < 1; ++j)
    //        output->matrix[i][j] = i;

    //TrainingData data = {input, output};
    //Matrix **nablab, **nablaw;
    //backprop(network, &data, &nablab, &nablaw);

    //printf("printing nablas\n");
    //for (size_t i = 0; i < network->num_layers - 1; ++i)
    //{
    //    printf("nabla_b:\n");
    //    print_matrix2(nablab[i]);
    //    printf("nabla_w:\n");
    //    print_matrix2(nablaw[i]);
    //    free_matrix2(nablab[i]);
    //    free_matrix2(nablaw[i]);
    //}
    //free(nablab);
    //free(nablaw);
    //free_matrix2(output);

    //NN_free_network(network);
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
    //size_t neurons[] = {4, 8, 2, 5};
    //NN_Network *network = NN_create_network(neurons, 4);

    //Matrix *input = init_matrix2(4, 1);

    //for (size_t i = 0; i < 4; ++i)
    //    for (size_t j = 0; j < 1; ++j)
    //        input->matrix[i][j] = i;

    //Matrix *output = init_matrix2(5, 1);
    //for (size_t i = 0; i < 5; ++i)
    //    for (size_t j = 0; j < 1; ++j)
    //        output->matrix[i][j] = i;

    //NN_print_weights(network);
    //NN_print_biases(network);
    //TrainingData data = {input, output};
    //TrainingData *dataP = &data;
    //update_mini_batch(network, &dataP, 0, 1, XOR_ETA, XOR_LAMBDA);

    //NN_print_weights(network);
    //NN_print_biases(network);

    //free_matrix2(output);
    //NN_free_network(network);

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
    
    sdg(network, &dataP, 1, 200, 10, XOR_ETA, NULL, 0, XOR_LAMBDA);

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

    sdg(network, data, 4, XOR_EPOCHS, XOR_MINI_BATCH_SIZE, XOR_ETA, data, 4,
            XOR_LAMBDA);

    save_network(network, "xor.ai");
    for (size_t i = 0; i < 4; ++i)
    {
        free_training_data(data[i]);
    }
    free(data);
    NN_free_network(network);
}

void try_xor(int argc, char **data)
{
    if (argc != 2)
        return;
    Matrix *input = init_matrix2(2, 1);
    input->matrix[0][0] = data[0][0] - '0';
    input->matrix[1][0] = data[1][0] - '0';
    printf("Input is \n");
    print_matrix2(input);
    printf("Loading network...\n");
    NN_Network *n = load_network("xor.ai");
    printf("Searching value\n");
    Matrix *output = NN_feedforward(n, input);
    int res = argmax(output);
    printf("Result is %i\n", res);

    free_matrix2(output);
    NN_free_network(n);
}

void test_digit()
{
    size_t neurons[] = {784, 32, 10};

    NN_Network *network = NN_create_network(neurons, 3);

    TrainingData **data, **tests;
    printf("Loading training set...\n");
    size_t n = load_images(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);
    printf(" Done !\n");
    printf("Loading test set...\n");
    size_t n_tests = load_images(TEST_SET_IMAGE, TEST_SET_LABEL, &tests);
    printf(" Done !\n");
    printf("Starting Scalar Gradient descent\n");
    sdg(network, data, n, DIGITS_EPOCHS,DIGITS_MINI_BATCH_SIZE, DIGITS_ETA,
            tests, n_tests, DIGITS_LAMBDA);

    NN_free_network(network);
    free_all_training_data(tests, n_tests);
    free_all_training_data(data, n);
}

void train_load_digit(char *path)
{
    NN_Network *network = load_network(path);
    TrainingData **data, **tests;
    printf("Loading training set...\n");
    size_t n = load_images(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);
    printf(" Done !\n");
    printf("Loading test set...\n");
    size_t n_tests = load_images(TEST_SET_IMAGE, TEST_SET_LABEL, &tests);
    printf(" Done !\n");
    printf("Starting Scalar Gradient descent\n");
    sdg(network, data, n, DIGITS_EPOCHS,DIGITS_MINI_BATCH_SIZE, DIGITS_ETA,
            tests, n_tests, DIGITS_LAMBDA);

    free(network->sizes);
    NN_free_network(network);
    free_all_training_data(tests, n_tests);
    free_all_training_data(data, n);

}

void test_mnist_reader()
{
    TrainingData **data;
    size_t n = load_images(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);

    for (size_t i = 0; i < n; ++i)
    {
        print_image(data[i]->image);
        print_matrix2(data[i]->expected);
    }
    free_all_training_data(data, n);
}

void test_save(char *path)
{
    size_t neurons[] = {784, 32, 10};

    NN_Network *network = NN_create_network(neurons, 3);
    save_network(network, path);
    NN_free_network(network);
    

    NN_Network *n = load_network(path);
    free(n->sizes);
    NN_free_network(n);
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
    else if (strcmp(argv[1], "--mnist") == 0)
        test_mnist_reader();
    else if (strcmp(argv[1], "--digits") == 0)
        test_digit();
    else if (strcmp(argv[1], "--save") == 0)
        test_save(argv[2]);
    else if (strcmp(argv[1], "--loadd") == 0)
        train_load_digit(argv[2]);
    else if (strcmp(argv[1], "--tryxor") == 0)
        try_xor(argc - 2, argv + 2);
    else
        print_help();

    return EXIT_SUCCESS;
}

