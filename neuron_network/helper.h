# pragma once

# include <stdlib.h>
# include <stddef.h>
# include <math.h>
# include <assert.h>
# include <string.h>
# include <stdio.h>


// represent a matrix of size m x n
typedef struct Matrix
{
    double **matrix;
    size_t m;
    size_t n;
    size_t id;
} Matrix;

typedef struct TrainingData
{
    // An array of pixels (28 * 28) = 784
    Matrix *image;

    // what the image represents
    // here, an array of 10, each index, the number
    // 0 should be {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    // 8 should be {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    Matrix *expected;
    
} TrainingData;


/**
  * Derivative of the sigmoid function
  * A a matrix of dim (m * 1)
  */
void sigmoid_prime(double **a, size_t m);
void sigmoid_prime2(Matrix *a);

double sigmoid(double x);

double random_value();

/**
  * Compute the sigmoid of a layer input
  * A and B are expected to be matrices of size (m * 1)
  * A is modified and holds the computation
  */
void sigmoid_of_matrix(double **a, double **biases, size_t m);
void sigmoid_of_matrix2(Matrix *a, Matrix *b);

/**
  * Similare to sigmoid_of_matrix but we assume
  * that the biases were already added.
  */
void sigmoid_matrix(double **a, size_t m);
void sigmoid_matrix2(Matrix *a);


/**
  * Calculate vector product
  * If both a and b are 1-D arrays, it is inner product of vectors
  * (without complex conjugation).
  * Allocate a new array of size n to hold the result and returns it
  */
double *dot_1d(const double *a, const double *b, size_t n);

/**
  * Calculate matrix product
  * If A is an m x n matrix
  *    B a n * p matrix
  * then AB is a m x p matrix
  * AB is allocated in the memory
  * 1d array as 2d
  */
double **dot_2d(double **a, size_t m, size_t n, double **b, size_t
        p);
Matrix *dot_2d2(Matrix *a, Matrix *b);

void swap(void *a, void *b, size_t data_size);


void shuffle(void* a, size_t n, size_t data_size);

void print_matrix(double **a, size_t m, size_t n);
void print_matrix2(Matrix *a);

/**
  * Allocate the transposed of the given matrix of size m * n
  * Result, a matrix of size n * m
  */
double **transpose(double **a, size_t m, size_t n);
Matrix *transpose2(Matrix *a);

/**
  * Create and heap allocate a matrix of size m * n
  * Accessible with matrix[m][n]
  */
double **init_matrix(size_t m, size_t n);
Matrix *init_matrix2(size_t m, size_t n);

/**
  * Free a matrix created by init_matrix of size m
  */
void free_matrix(double **a, size_t m);
void free_matrix2(Matrix *a);

/**
  * Add to matrices A and B of same dimensions m * n
  * The result is saved in A
  */
void add_matrix(double **a, double **b, size_t m, size_t n);
void add_matrix2(Matrix *a, Matrix *b);
void add_matrix_double(Matrix *a, double b);

/**
  * Subtract B to A, matrices A and B of same dimensions m * n
  * The result is saved in A
  */
void sub_matrix(double **a, double **b, size_t m, size_t n);
void sub_matrix2(Matrix *a, Matrix *b);
/**
  * Same as add_matrix but instead, A and B are not modified
  * and a new matrix holding the sum result is allocated (m * n)
  */
double **add_matrix_heap(double **a, double **b, size_t m, size_t n);
Matrix *add_matrix_heap2(Matrix *a, Matrix *b);

/**
  * Copy an allocate a new heap matrix of dimension m * n
  * from a given matrix A of dim (m * n)
  */
double **copy_matrix(double **a, size_t m, size_t n);
Matrix *copy_matrix2(Matrix *a);
/**
  * Simply perform the product of a[i][j] with b[i][j]
  * A and B two matrices of same dimension m x n
  * Result in A
  */
void mul_matrix(double **a, double **b, size_t m, size_t n);
void mul_matrix2(Matrix *a, Matrix *b);
void mul_matrix_scalar(Matrix *a, double scalar);


/**
  * Returns the index of the maximum of a (m x 1) matrix
  */
int argmax(Matrix *a);

/**
  * Returns a heap list of size n the indexes of the maximums of a (m x 1) matrix,
  * where m could be various sizes.
  */
int *argmax_matrices(Matrix **l, size_t n);


void free_training_data(TrainingData *data);

void norm_matrix(double** matrix, size_t m, size_t n);
void norm_matrix2(Matrix *a);
