#define _GNU_SOURCE

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <string.h>
# include <time.h>
# include <mcheck.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "mnist_reader.h"

# include "Neural_Network/second_network.h"
# include "Neural_Network/network_loader.h"

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
           "       --s [path] :      Train and save the network at path\n"
           "       --l [path] :      Load the network at path\n"
           "       --lr [path] :     Load and run the network at path\n"
          );
}

char print_img(Matrix *a)
{
    char f = 0;
    for (size_t i = 0; i < a->w; i++)
        for (size_t j = 0; j < a->h; j++)
            if (a->values[i][j] != 0) {
                f = 1;
                break;
            }
    if (!f)
        return 0;
    for (size_t i = 0; i < 28; ++i) {
        for (size_t j = 0; j < 28; ++j)
            if (a->values[0][i * 28 + j] > 0)
                printf("@ ");
            else
                printf(". ");
        printf("\n");
    }
    return 1;
}

Tuple_m* Load_Images(char* path, size_t* index, size_t num)
{
    Tuple_m* arr = malloc(sizeof(Tuple_m) * num * 10);
    *index = 0;
    for (size_t i = 0; i <= 9; i++) {
        for (size_t j = 0; j < num; j++) {
            char* s;
            if (asprintf(&s, "%s/%zu/%zu.png", path, i, j) != -1) {
                SDL_Surface* surf = IMG_Load(s);
                if (!surf) {
                    free(s);
                    continue;
                }
                Matrix* m = new_Matrix(1, 784);
                for (size_t k = 0; k < 784; k++) {
                    m->values[0][k] = (*(Uint32*)(surf->pixels + k)) > INT32_MAX / 2? 1 : 0;
                }
                Matrix* res = new_Matrix(1,10);
                res->values[0][i] = 1;
                arr[*index].x = m;
                arr[*index].y = res;
                *index += 1;
                //print_img(m);
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
    /*
    for (size_t i = 0; i < 50000; i++)
    {
        char* c;
        asprintf(&c, "database/testing/0/%zu.png", i);
        SDL_Surface* s = IMG_Load(c);
        if (!s)
        {
            free(c);
            continue;
        }

        for (size_t j = 0; j < 784; j++)
        {
            if (rand() / (double)RAND_MAX < 0.002)
            {
                *(Uint32*)(s->pixels + j) = -1;
            }
            else
            {
                *(Uint32*)(s->pixels + j) = 0;
            }
        }

        IMG_SavePNG(s, c);
        SDL_FreeSurface(s);
        free(c);
    }
    for (size_t i = 0; i < 50000; i++)
    {
        char* c;
        asprintf(&c, "database/training/0/%zu.png", i);
        SDL_Surface* s = IMG_Load(c);
        if (!s)
        {
            free(c);
            continue;
        }

        for (size_t j = 0; j < 784; j++)
        {
            if ((rand() / (double)RAND_MAX) < 0.002)
            {
                *(Uint32*)(s->pixels + j) = -1;
            }
            else
            {
                *(Uint32*)(s->pixels + j) = 0;
            }
        }

        IMG_SavePNG(s, c);
        SDL_FreeSurface(s);
        free(c);
    }*/
    clock_t t = clock();
    size_t neurons[] = {784, 32, 10};
    Network *network = new_Network(neurons, 3);
    /*size_t n;
    Tuple_m* data = Load_Images("database/training", &n, 5000);*/
    /*size_t n_tests;
    Tuple_m* tests = Load_Images("database/testing", &n_tests, 10000);*/
    size_t n = 68500 + 2840;
    size_t n_tests = 18500 + 2840;
    Tuple_m* data = malloc(n * sizeof(Tuple_m));
    Tuple_m* tests = malloc(n_tests * sizeof(Tuple_m));
    printf("Loading MNIST data...\n");
    load_mnist();
    printf("Loading complete in %f\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Loading training set...\n");
    for (size_t i = 0; i < 60000; i++) {
        Matrix* m = new_Matrix(1, 784);
        for (size_t j = 0; j < 784; j++)
            m->values[0][j] = train_image[i][j];
        Matrix* res = new_Matrix(1, 10);
        res->values[0][train_label[i]] = 1;
        if (train_label[i] == 0) {
            for (size_t j = 0; j < 784; j++)
                if (rand() / (double)RAND_MAX > 0.98)
                    m->values[0][j] = 1;
                else
                    m->values[0][j] = 0;
        } else {
            for (size_t j = 0; j < 784; j++)
                if (rand() / (double)RAND_MAX > 0.98)
                    m->values[0][j] = 1;
        }
        data[i].x = m;
        data[i].y = res;
    }
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Loading test set...\n");
    for (size_t i = 0; i < 10000; i++) {
        Matrix* m = new_Matrix(1, 784);
        for (size_t j = 0; j < 784; j++)
            m->values[0][j] = test_image[i][j];
        Matrix* res = new_Matrix(1, 10);
        res->values[0][test_label[i]] = 1;
        if (test_label[i] == 0) {
            for (size_t j = 0; j < 784; j++)
                if (rand() / (double)RAND_MAX > 0.98)
                    m->values[0][j] = 1;
                else
                    m->values[0][j] = 0;
        } else {
            for (size_t j = 0; j < 784; j++)
                if (rand() / (double)RAND_MAX > 0.98)
                    m->values[0][j] = 1;
        }
        tests[i].x = m;
        tests[i].y = res;
    }
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Loading additional datasets...\n");
    for (size_t j = 0; j < 10; j++)
        for (size_t i = 0; i < 85; i++) {
            char* c;
            if (asprintf(&c, "database/training_set/%zu/%zu.png", j, i) == -1)
                errx(EXIT_FAILURE, "asprintf failed");
            SDL_Surface* su = IMG_Load(c);
            if (!su)
                continue;
            for (size_t l = 0; l < 10; l++) {
                Matrix* m = new_Matrix(1, 784);
                for (size_t k = 0; k < 784; k++) {
                    Uint32 v = (*(Uint32*)(su->pixels + k));
                    m->values[0][k] = v ? 1 : 0;
                    if (rand() / (double)RAND_MAX > 0.98)
                        m->values[0][k] = 1;
                }
                Matrix* res = new_Matrix(1, 10);
                res->values[0][j] = 1;
                data[i*100 + l*10 + j + 60000].x = m;
                data[i*100 + l*10 + j + 60000].y = res;
            }
            for (size_t l = 0; l < 10; l++) {
                Matrix* m = new_Matrix(1, 784);
                for (size_t k = 0; k < 784; k++) {
                    m->values[0][k] = (*(Uint32*)(su->pixels + k)) ? 1 : 0;
                    if (rand() / (double)RAND_MAX > 0.98)
                        m->values[0][k] = 1;
                }
                Matrix* res = new_Matrix(1, 10);
                res->values[0][j] = 1;
                tests[i*100 + l*10 + j + 10000].x = m;
                tests[i*100 + l*10 + j + 10000].y = res;
                /* print_img(m);
                printf("------>    %i\n\n\n\n\n", test_label[i]); */
            }
            SDL_FreeSurface(su);
        }
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 284; j++) {
            char* s;
            if (asprintf(&s, "database/training2/%zu-%04zu.png", i, j) == -1)
                errx(EXIT_FAILURE, "asprintf failed");
            SDL_Surface* su = IMG_Load(s);
            if (!su) {
                free(s);
                break;
            }
            Matrix* m = new_Matrix(1, 784);
            for (size_t k = 0; k < 784; k++) {
                Uint32 v = (*(Uint32*)(su->pixels + k));
                m->values[0][k] = v ? 1 : 0;
                if (rand() / (double)RAND_MAX > 0.98)
                    m->values[0][k] = 1;
            }
            Matrix* res = new_Matrix(1, 10);
            res->values[0][i] = 1;
            data[68500 + j * 10 + i].x = m;
            data[68500 + j * 10 + i].y = res;
            Matrix* m2 = new_Matrix(1, 784);
            for (size_t k = 0; k < 784; k++) {
                Uint32 v = (*(Uint32*)(su->pixels + k));
                m2->values[0][k] = v ? 1 : 0;
                if (rand() / (double)RAND_MAX > 0.98)
                    m2->values[0][k] = 1;
            }
            Matrix* res2 = new_Matrix(1, 10);
            res2->values[0][i] = 1;
            tests[18500 + j * 10 + i].x = m2;
            tests[18500 + j * 10 + i].y = res2;
            SDL_FreeSurface(su);
            free(s);
        }
    }
    printf("Extra sets loaded in %fs\n", (double)(clock() - t) / 1000000);
    t = clock();
    printf("Starting Stochastic Gradient Descent\n");
    while (1) {
        SGD(network, data, n, 1, 10, 1, tests, n_tests);
        Save_Network(network, "networko");
    }
    printf("Calculated weights and biases in %fs\n", (double)(clock() - t)/1000000);
    Free_Network(network);
    printf("Freeing\n");
    for (size_t i = 0; i < n; i++) {
        free_m(data[i].x);
        free_m(data[i].y);
    }
    for (size_t i = 0; i < n_tests; i++) {
        free_m(tests[i].x);
        free_m(tests[i].y);
    }
    free(tests);
    free(data);
}

void load_digit(char* path)
{
    Network* n = Load_Network(path);
    printf("\n\n\n\n\n\nWeights = \n\n\n");
    print_list_m(n->weights);
    printf("Biases =\n");
    print_list_m(n->biases);
    printf("\n\n\nNetwork is\n{\n");
    print_list(n->sizes);
    printf("}\n");
    Free_Network(n);
}

void train_load_digit(char *path)
{
    Network *network = Load_Network(path);
    clock_t t = clock();
    printf("Loading training set...\n");
    size_t n;
    Tuple_m* data = Load_Images("database/training", &n, 5000);
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Loading test set...\n");
    size_t n_tests;
    Tuple_m* tests = Load_Images("database/testing", &n_tests, 10000);
    printf("Done in %fs\n", (double)(clock() - t)/1000000);
    t = clock();
    printf("Starting Stochastic Gradient Descent\n");
    print_list_m(network->biases);
    printf("\n\n\n\n\n\n\n\nWeights\n\n\n");
    print_list_m(network->weights);
    printf("\n\nDone loading...\n");
    while (1) {
        SGD(network, data, n, 1, 10, 1, tests, n_tests);
        Save_Network(network, "networko");
    }
    printf("Calculated weights and biases in %fs\n", (double)(clock() - t)/1000000);
    Free_Network(network);
    printf("Freeing\n");
    for (size_t i = 0; i < n; i++) {
        free_m(data[i].x);
        free_m(data[i].y);
    }
    for (size_t i = 0; i < n_tests; i++) {
        free_m(tests[i].x);
        free_m(tests[i].y);
    }
    free(tests);
    free(data);
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        print_help();
        return EXIT_SUCCESS;
    }
    srand(SEED);
    printf("Starting test suite...\n");
    if (strcmp(argv[1], "--s") == 0)
        test_digit();
    else if (strcmp(argv[1], "--l") == 0)
        load_digit(argv[2]);
    else if (strcmp(argv[1], "--lr") == 0)
        train_load_digit(argv[2]);
    else
        print_help();
    return EXIT_SUCCESS;
}
