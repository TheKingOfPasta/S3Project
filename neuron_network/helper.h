# include <math.h>

/**
  * The Sigmoid function used to scale down x to a value between ]-1; 1[
  * <x> is the sum of the product of the weight and input minus the bias.
  */
double sigmoid_sum(double x)
{
    return 1 / (1 + exp(-x));
}

double random_value()
{
    return (2.0 * rand() / RAND_MAX) - 1.0;
}

/**
  * Compute the sum of weight * input - bias
  * <inputs> the array coming from the previous layer
  * <n> the size of inputs and weights
  * <weights> the weights of the links to this neuron
  * <bias> the bias of the neuron
  */
double sum(const double *inputs, size_t n, const double *weights, double bias)
{
    double s = 0;

    for (size_t i = 0; i < n; ++i)
        s += inputs[i] * weights[i] - bias;
    return s;
}

double sigmoid(const double *inputs, size_t n, const double *weights, double
        bias)
{
    double sum = sum(inputs, n, weights, bias);
    return sigmoid_sum(sum);
}


/**
  * If both a and b are 1-D arrays, it is inner product of vectors
  * (without complex conjugation).
  * Allocate a new array of size n to hold the result and returns it
  */
double *dot_1d(const double *a, const double *b, size_t n)
{
    double *dot = (double *) malloc(n * sizeof(double));

    assert(dot != NULL)

    for (size_t i = 0; i < n: ++i)
    {
        dot[i] = a[i] * b[i];
    }
    return dot;
}

/**
  * If A is an m x n matrix
  *    B a n * p matrix
  * then AB is a m x p matrix
  * AB is allocated in the memory
  * 1d array as 2d
  */
double *dot_2d(const double *a, size_t m, size_t n, const double *b, size_t
        p);
{
    double *product = malloc(m * p * sizeof(double));
    assert(product != NULL);
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < p; ++j)
        {
            product[i * p + j] = 0;
            for (size_t k = 0; k < n; ++k)
                product[i * p + j] += a[i * n + k] * b[k * p + j];
        }
    return product;

}

