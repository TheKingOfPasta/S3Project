# pragma once
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <string.h>
# define INPUT_LAYER 784
# define HIDDEN_LAYER 30
# define OUTPUT_LAYER 10
# define NUM_LAY 3
# define ETA 3
# define MINI_BATCH_SIZE 30
# define EPOCHS 30

typedef struct Mat
{
    size_t row;
    size_t col;
    float *arr;
} Mat;

typedef struct Network
{
    Mat **weights;
    Mat **biases;

} Network;

typedef struct Data
{
    float *image;
    float *expected;
} Data;

void print_arr(float *arr, size_t n);
