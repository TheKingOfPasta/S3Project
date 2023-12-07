#include "inverse.h"

/*For calculating Determinant of the Matrix */
double determinant(Matrix* m1)
{
    double s = 1, det = 0;
    Matrix* b = new_Matrix(m1->w - 1, m1->h - 1);
    int i, j, m, n, c;
    if (m1->w == 1)
    {
        return (m1->values[0][0]);
    }
    else
    {
        det = 0;
        for (c = 0; c < m1->w; c++)
        {
            m = 0;
            n = 0;
            for (i = 0; i < m1->w; i++)
            {
                for (j = 0; j < m1->w; j++)
                {
                    b->values[i][j] = 0;
                    if (i != 0 && j != c)
                    {
                        b->values[m][n] = m1->values[i][j];
                        if (n < (m1->w - 2))
                            n++;
                        else
                        {
                            n = 0;
                            m++;
                        }
                    }
                }
            }
            det = det + s * (m1->values[0][c] * determinant(b));
            s = -1 * s;
        }
    }

    return (det);
}

Matrix* cofactor(Matrix* m1)
{
    Matrix* b = new_Matrix(m1->w - 1, m1->h - 1);
    Matrix* fac = new_Matrix(m1->w - 1, m1->h - 1);
    int p, q, m, n, i, j;
    for (q = 0; q < m1->w; q++)
    {
        for (p = 0; p < m1->w; p++)
        {
            m = 0;
            n = 0;
            for (i = 0; i < m1->w; i++)
            {
                for (j = 0; j < m1->w; j++)
                {
                    if (i != q && j != p)
                    {
                        b->values[m][n] = m1->values[i][j];
                        if (n < (m1->w - 2))
                            n++;
                        else
                        {
                            n = 0;
                            m++;
                        }
                    }
                }
            }
            fac->values[q][p] = pow(-1, q + p) * determinant(b);
        }
    }
    return transpose(m1, fac);
}

/*Finding transpose of matrix*/
Matrix* transpose(Matrix* m1, Matrix* m2)
{
    int i, j;
    double d;
    Matrix* b = new_Matrix(m1->w, m1->h);
    Matrix* inverse = new_Matrix(m1->w, m1->h);
    for (i = 0; i < m1->w; i++)
    {
        for (j = 0; j < m1->w; j++)
        {
            b->values[i][j] = m2->values[j][i];
        }
    }
    d = determinant(m1);
    for (i = 0; i < m1->w; i++)
    {
        for (j = 0; j < m1->w; j++)
        {
            inverse->values[i][j] = b->values[i][j] / d;
        }
    }
    printf("\n\n\nThe inverse of matrix is : \n");

    print_m(inverse);

    return inverse;
}

int main()
{
    double k, d;
    int i, j;
    printf("Enter the order of the Matrix : ");
    Matrix* a = new_Matrix(3, 3);
    print_m(a);
    printf("Enter the elements of %.0fX%.0f Matrix : \n", k, k);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            a->values[i][j] = i * 3 + j;
        }
    }
    print_m(a);
    d = determinant(a);
    printf("determinant : %f\n", d);
    if (d == 0)
        printf("\nInverse of Entered Matrix is not possible\n");
    else
        print_m(cofactor(a));
}