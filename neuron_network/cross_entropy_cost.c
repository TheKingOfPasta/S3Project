# include "helper.h"
# include <math.h>
# include <err.h>


void cross_entropy(Matrix *a, Matrix *y)
{
    if (a->m != y->m || a->n != y->n)
        errx(EXIT_FAILURE, "wrong dimensions");
    for (size_t i = 0; i < a->m; ++i)
    {
        for (size_t j = 0; j < a->n; ++j)
        {
            double log_a = log(a->matrix[i][j]);
            double log_ainv = log(1 - a->matrix[i][j]);
            double val_y = y->matrix[i][j];
            a->matrix[i][j] = -val_y * log_a - (1 - val_y) * log_ainv;
            // maybe if nan, set to 0 (very small value)
        }
    }
}

void cross_entropy_delta(Matrix *a, Matrix *y)
{
    sub_matrix2(a, y);
}
