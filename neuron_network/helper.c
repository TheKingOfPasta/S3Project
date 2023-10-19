# include "helper.h"
# include <stdlib.h>
# include <stddef.h>
# include <math.h>
# include <assert.h>
# include <string.h>

/**
  * The Sigmoid function used to scale down x to a value between ]-1; 1[
  * <x> is the sum of the product of the weight and input minus the bias.
  */
double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

/**
  * Compute the sigmoid of a layer input
  * A and B are expected to be matrices of size (m * 1)
  * A is modified and holds the computation
  */
void sigmoid_of_matrix(double **a, double **biases, size_t m)
{
    for (size_t i = 0; i < m; ++i)
    {
        a[i][0] = sigmoid(a[i][0] + biases[i][0]);
    }
}

/**
  * Similare to sigmoid_of_matrix but we assume
  * that the biases were already added.
  */
void sigmoid_matrix(double **a, size_t m)
{
    for (size_t i = 0; i < m; ++i)
        a[i][0] = sigmoid(a[i][0]);
}

/**
  * Derivative of the sigmoid function
  * A a matrix of dim (m * 1)
  */
void sigmoid_prime(double **a, size_t m)
{
    for (size_t i = 0; i < m; ++i)
    {
        double sig = sigmoid(a[i][0]);
        a[i][0] = sig * (1 - sig);
    }
}

double random_value()
{
    return (2.0 * rand() / RAND_MAX) - 1.0;
}


///**
//  * Compute the sum of weight * input - bias
//  * <inputs> the array coming from the previous layer
//  * <n> the size of inputs and weights
//  * <weights> the weights of the links to this neuron
//  * <bias> the bias of the neuron
//  */
//double sum(const double *inputs, size_t n, const double *weights, double bias)
//{
//
//    double s = 0;
//
//    for (size_t i = 0; i < n; ++i)
//        s += inputs[i] * weights[i] - bias;
//    return s;
//}
//
//
//double sigmoid(const double *inputs, size_t n, const double *weights, double
//        bias)
//{
//    double result = sum(inputs, n, weights, bias);
//    return sigmoid_sum(result);
//}
//
//double sigmoid_prime(const double *inputs, size_t n, const double *weights,
//        double bias)
//{
//    double sig = sigmoid(inputs, n, weights, bias);
//    return sig * (1 - sig);
//}


/**
  * Calculate vector product
  * If both a and b are 1-D arrays, it is inner product of vectors
  * (without complex conjugation).
  * Allocate a new array of size n to hold the result and returns it
  */
double *dot_1d(const double *a, const double *b, size_t n)
{
    double *dot = (double *) malloc(n * sizeof(double));

    assert(dot != NULL);

    for (size_t i = 0; i < n; ++i)
    {
        dot[i] = a[i] * b[i];
    }
    return dot;
}

/**
  * Calculate matrix product
  * If A is an m x n matrix
  *    B a n * p matrix
  * then AB is a m x p matrix
  * AB is allocated in the memory
  * 1d array as 2d
  */
double **dot_2d(double **a, size_t m, size_t n, double **b, size_t
        p)
{
    double **product = init_matrix(m, p);
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < p; ++j)
        {
            for (size_t k = 0; k < n; ++k)
                product[i][j] += a[i][k] * b[k][j];
        }
    }
    return product;

}

/**
  * Simply perform the product of a[i][j] with b[i][j]
  * A and B two matrices of same dimension m x n
  * Result in A
  */
void mul_matrix(double **a, double **b, size_t m, size_t n)
{
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            a[i][j] *= b[i][j];
}

/**
  * Free a matrix created by init_matrix of size m
  */
void free_matrix(double **a, size_t m)
{
    for (size_t i = 0; i < m; ++i)
    {
        free(a[i]);
    }
    free(a);
}

/**
  * Add to matrices A and B of same dimensions m * n
  * The result is saved in A
  */
void add_matrix(double **a, double **b, size_t m, size_t n)
{
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            a[i][j] += b[i][j];
}

/**
  * Subtract B to A, matrices A and B of same dimensions m * n
  * The result is saved in A
  */
void sub_matrix(double **a, double **b, size_t m, size_t n)
{
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            a[i][j] -= b[i][j];
}

/**
  * Same as add_matrix but instead, A and B are not modified
  * and a new matrix holding the sum result is allocated (m * n)
  */
double **add_matrix_heap(double **a, double **b, size_t m, size_t n)
{
    double **sum = init_matrix(m, n);
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            sum[i][j] = a[i][j] + b[i][j];
    return sum;
}

/**
  * Create and heap allocate a matrix of size m * n
  * Accessible with matrix[m][n]
  */
double **init_matrix(size_t m, size_t n)
{
    double **matrix = malloc(m * sizeof(double *));
    for (size_t j = 0; j < m; ++j)
    {
        matrix[j] = calloc(n, sizeof(double));
        assert(matrix[j] != NULL);
    }
    return matrix;
}

/**
  * Copy an allocate a new heap matrix of dimension m * n
  * from a given matrix A of dim (m * n)
  */
double **copy_matrix(double **a, size_t m, size_t n)
{
    double **copy = init_matrix(m, n);    
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            copy[i][j] = a[i][j];
    return copy;
}

/**
  * Allocate the transposed of the given matrix of size m * n
  *
  */
double **transpose(double **a, size_t m, size_t n)
{
    double **transposed = init_matrix(n, m);
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            transposed[j][i] = a[i][j];
    return transposed;
}

void print_matrix(double **a, size_t m, size_t n)
{
    printf("[\n");
    for (size_t i = 0; i < m; ++i)
    {
        printf("\t[");
        for (size_t j = 0; j < n; ++j)
            printf("%5.2f ", a[i][j]);
        printf("]\n");
    }
    printf("]\n");
}

void swap(void *a, void *b, size_t data_size)
{
    void *tmp = malloc(data_size);
    memcpy(tmp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, tmp, data_size);

    free(tmp);
}

void shuffle(void* a, size_t n, size_t data_size)
{
    for (size_t i = n - 1; i > 0; --i)
    {
        size_t j = arc4random_uniform(i);

        swap((void *)(a + i * data_size), (void *)(a + j * data_size), data_size);
    }
}


/**
  * Compute the cost function between two layers output:
  * - the output: what the neuron network gave back at this state
  * - the expected: what was expected instead
  * They're both arrays of size n
  * return the sum of the squared difference between each index
  */
double cost_function(double const *output,  double const*expected, size_t n)
{
    double cost = 0;

    for (size_t i = 0; i < n; ++i)
    {
        double diff = output[i] - expected[i];
        cost += diff * diff;
    }
    return cost;
}
