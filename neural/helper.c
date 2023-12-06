#include "helper.h"

double random_value()
{
    return (2.0 * rand() / RAND_MAX) - 1.0;
}

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

Matrix *dot_2d2(Matrix *a, Matrix *b)
{
    Matrix *product = malloc(sizeof(Matrix));
    product->values = dot_2d(a->values, a->w, a->h, b->values, b->h);
    product->w = a->w;
    product->h = b->h;
    return product;
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
void mul_matrix2(Matrix *a, Matrix *b)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "a (%zu x %zu) and b (%zu x %zu) dimensions does not"
                "match", a->w, a->h, b->w, b->h);
    mul_matrix(a->values, b->values, a->w, a->h);
}

void mul_matrix_scalar(Matrix *a, double scalar)
{
    for (size_t i = 0; i < a->w; ++i)
        for (size_t j = 0; j < a->h; ++j)
            a->values[i][j] *= scalar;
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

void free_matrix2(Matrix *a)
{
    free_matrix(a->values, a->w);
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

void add_matrix2(Matrix *a, Matrix *b)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "a (%zu x %zu) and b (%zu x %zu) dimensions does not"
                "match", a->w, a->h, b->w, b->h);
    add_matrix(a->values, b->values, a->w, a->h);
}

void add_matrix_double(Matrix *a, double b)
{
    for (size_t i = 0; i < a->w; ++i)
        for (size_t j = 0; j < a->h; ++j)
            a->values[i][j] += b;
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

void sub_matrix2(Matrix *a, Matrix *b)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "a (%zu x %zu) and b (%zu x %zu) dimensions does not"
                "match", a->w, a->h, b->w, b->h);

    sub_matrix(a->values, b->values, a->w, a->h);
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
Matrix *add_matrix_heap2(Matrix *a, Matrix *b)
{
    if (a->w != b->w || a->h != b->h)
        errx(EXIT_FAILURE, "a (%zu x %zu) and b (%zu x %zu) dimensions does not"
                "match", a->w, a->h, b->w, b->h);
    Matrix *sum = malloc(sizeof(Matrix));
    sum->values = add_matrix_heap(a->values, b->values, a->w, a->h);
    sum->w = a->w;
    sum->h = a->h;
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

Matrix *init_matrix2(size_t m, size_t n)
{
    Matrix *a = malloc(sizeof(Matrix));
    a->values = init_matrix(m, n);
    a->w = m;
    a->h = n;
    return a;
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

Matrix *copy_matrix2(Matrix *a)
{
    Matrix *copy = malloc(sizeof(Matrix));
    copy->values = copy_matrix(a->values, a->w, a->h);
    copy->w = a->w;
    copy->h = a->h;
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

Matrix *transpose2(Matrix *a)
{
    Matrix *aT = malloc(sizeof(Matrix));
    aT->values = transpose(a->values, a->w, a->h);
    aT->w = a->h;
    aT->h = a->w;
    return aT;
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

void print_matrix2(Matrix *a)
{
    print_matrix(a->values, a->w, a->h);
}

void swap(void *a, void *b, size_t data_size)
{
    void *tmp = malloc(data_size);
    memcpy(tmp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, tmp, data_size);

    free(tmp);
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

/**
  * Returns the index of the maximum of a (m x 1) matrix
  */
int argmax(Matrix *a)
{
    if (a->h != 1)
        errx(EXIT_FAILURE, "argmax() is expecting a matrix (%zux1), received"
                "(%zux%zu)\n", a->w, a->h, a->h);
    int index = 0;
    for (size_t i = 1; i < a->w; ++i)
        if (a->values[i][0] > a->values[index][0])
            index = i;
    return index;
}

/**
  * Returns a heap list of size n the indexes of the maximums of a (m x 1) matrix,
  * where m could be various sizes.
  */

int *argmax_matrices(Matrix **l, size_t n)
{
    int *list = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; ++i)
        list[i] = argmax(l[i]);
    return list;
}

void free_training_data(Tuple_m *data)
{
    free_matrix2(data->y);
    free_matrix2(data->x);
    free(data);
}

void norm_matrix(double** matrix, size_t m, size_t n)
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; ++j)
        {
	    double tmp = exp(-(matrix[i][j]*matrix[i][j])/2)/sqrt(2*M_PI);
	    if (matrix[i][j] < 0)
		tmp = -tmp;
            matrix[i][j] = tmp / sqrt(n);
        }
    }
}

void norm_matrix2(Matrix *a)
{
    norm_matrix(a->values, a->w, a->h);
}

//Matrix *broadcast_matrix(Matrix *a, Matrix *b)
//{
//    size_t 
//    if (a->w == 1)
//    Matrix *b = init_matrix
//}
