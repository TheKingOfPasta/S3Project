#define _GNU_SOURCE

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include <time.h>
# include <mcheck.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

# include "second_network.h"
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
    Network *network = new_Network(neurons, 4);

    Matrix *input = new_Matrix(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->values[i][j] = i;

    print_m(input);
    Matrix *output = feedforward(network, input);

    printf("output: \n");
    print_m(output);
    free_m(input);
    free_m(output);

    Free_Network(network);
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

    //print_m(input);
    //Matrix **zs;
    //Matrix **output = NN_feedforward_save(network, input, &zs);

    //printf("printing output: \n");
    //for (size_t i = 0; i < 4; ++i)
    //{
    //    print_m(output[i]);
    //    free_matrix2(output[i]);
    //}
    //free(output);

    //printf("printing zs: \n");
    //for (size_t i = 0; i < 3; ++i)
    //{
    //    print_m(zs[i]);
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
    //    print_m(nablab[i]);
    //    printf("nabla_w:\n");
    //    print_m(nablaw[i]);
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
    Network *network = new_Network(neurons, 4);
    print_list_m(network->biases);
    print_list_m(network->weights);

    Free_Network(network);
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
    Network *network = new_Network(neurons, 4);

    Matrix *input = new_Matrix(4, 1);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 1; ++j)
            input->values[i][j] = i;

    Matrix *output = new_Matrix(5, 1);
    for (size_t i = 0; i < 5; ++i)
        for (size_t j = 0; j < 1; ++j)
            output->values[i][j] = i;

    print_list_m(network->biases);
    print_list_m(network->weights);

    Tuple_m data;
    data.x = input;
    data.y = output;

    SGD(network, &data, 1, 200, 10, XOR_ETA, NULL, 0);

    print_list_m(network->biases);
    print_list_m(network->weights);

    free_m(input);
    free_m(output);
    Free_Network(network);

}

Tuple_m* Load_Images(size_t* index)
{
    // num = the number of pics per digit we choose, 0 < num < 3500
    size_t num = 1000;
    Tuple_m* arr = malloc(sizeof(Tuple_m) * num * 10);
    *index = 0;

    for (size_t i = 0; i <= 9; i++)
    {
        for (size_t j = 0; j < num; j++)
        {
            char* s;
            if (asprintf(&s, "database/%zu_%zu.png", i, j) != -1)
            {
                SDL_Surface* surf = IMG_Load(s);
                if (!surf)
                    continue;

                Uint32* pixels = surf->pixels;

                Matrix* m = new_Matrix(1, 784);

                for (size_t k = 0; k < 784; k++)
                {
                    Uint8 col;
                    int offset = (k/28) * surf->pitch + (k%28) * surf->format->BytesPerPixel;
                    SDL_GetRGB((*(Uint32*)((Uint8*)pixels + offset)), surf->format, &col, &col, &col);
                    m->values[0][k] = col + 1 < 0.1 ? 1 : 0;//-1 == white
                }

                Matrix* res = new_Matrix(1,10);
                res->values[0][i] = 1;

                arr[*index].x = m;
                arr[*index].y = res;

                *index += 1;

                SDL_FreeSurface(surf);
            }
            free(s);
        }
    }

    arr = realloc(arr, (*index) * sizeof(Tuple_m));

    return arr;
}

#include <time.h>

void test_digit()
{
    clock_t t = clock();
    size_t neurons[] = {784, 32, 10};

    Network *network = new_Network(neurons, 3);

    printf("Loading training set...\n");
    size_t n;
    Tuple_m* data = Load_Images(&n);
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Loading test set...\n");
    size_t n_tests;
    Tuple_m* tests = Load_Images(&n_tests);
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Starting Scalar Gradient descent\n");
    SGD(network, data, n, 30, DIGITS_MINI_BATCH_SIZE, DIGITS_ETA,
            tests, n_tests);

    printf("Calculated weights and biases in %fs\n", (double)(clock() - t)/1000000);

    save_network(network, "networko");

    printf("Biases\n");
    print_list_m(network->biases);
    printf("\n\n\n\nWeights\n");
    print_list_m(network->weights);

    Free_Network(network);

    printf("Freeing\n");
    for (size_t i = 0; i < n; i++)
    {
        free_m(data[i].x);
        free_m(data[i].y);
    }

    for (size_t i = 0; i < n_tests; i++)
    {
        free_m(tests[i].x);
        free_m(tests[i].y);
    }
    free(tests);
    free(data);
}

void train_load_digit(char *path)
{
    Network *network = load_network(path);
    Tuple_m *data, *tests;
    printf("Loading training set...\n");
    size_t n = load_images_m(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);
    printf(" Done !\n");
    printf("Loading test set...\n");
    size_t n_tests = load_images_m(TEST_SET_IMAGE, TEST_SET_LABEL, &tests);
    printf(" Done !\n");
    printf("Starting Scalar Gradient descent\n");
    SGD(network, data, n, DIGITS_EPOCHS, DIGITS_MINI_BATCH_SIZE, DIGITS_ETA,
                tests, n_tests);

    Free_List(network->sizes);
    Free_Network(network);
    //free_all_training_data(tests, n_tests);
    //free_all_training_data(data, n);
}

void test_mnist_reader()
{
    Tuple_m *data;
    size_t n = load_images_m(TRAINING_SET_IMAGE, TRAINING_SET_LABEL, &data);

    for (size_t i = 0; i < n; ++i)
    {
        print_image(data[i].x);
        print_m(data[i].y);
    }
    //free_all_training_data(data, n);
}

void test_save(char *path)
{
    size_t neurons[] = {784, 32, 10};

    Network *network = new_Network(neurons, 3);
    save_network(network, path);
    Free_Network(network);

    Network *n = load_network(path);
    Free_List(n->sizes);
    Free_Network(n);
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
        {
            
        }
    else if (strcmp(argv[1], "--mnist") == 0)
        test_mnist_reader();
    else if (strcmp(argv[1], "--digits") == 0)
        test_digit();
    else if (strcmp(argv[1], "--save") == 0)
        test_save(argv[2]);
    else if (strcmp(argv[1], "--loadd") == 0)
        train_load_digit(argv[2]);
    else if (strcmp(argv[1], "--tryxor") == 0)
        {
            
        }
    else
        print_help();

    return EXIT_SUCCESS;
}