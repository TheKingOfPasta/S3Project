# include "helper.h"
# include <math.h>

double quadratic(Matrix *a, Matrix *y)
{
    if (a->m != y->m || a->n != y->n)
        errx(EXIT_FAILURE, "Dimensions does not match");
    double sum = 0;
    for (size_t i = 0; i < a->m; ++i)
    {
        for (size_t j = 0; j < a->n ++j)
        {
            double diff = a->matrix[i][j] - y->matrix[i][j];

            // Frobenius norm
            sum += diff * diff;
        }
    }
    double norm = sqrt(sum);
    return 0.5 * norm * norm;
}

void delta(Matrix *z, Matrix *a, Matrix *y)
{
    for (size_t i = 0; i < a->m; ++i)
    {
        for (size_t j = 0; j < a->n ++j)
        {
            double diff = a->matrix[i][j] - y->matrix[i][j];
            double sig = sigmoid(z[i][j]);
            //               sigmoid_prime
            z[i][j] = diff * sig * (1 - sig);
            // Frobenius norm
        }
    }
    return sqrt(sum);
}
