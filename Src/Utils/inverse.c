#include "Utils/inverse.h"

/*For calculating Determinant of the Matrix */
double determinant(Matrix* m1)
{
    double s = 1, det = 0;
    Matrix* b = new_Matrix(m1->w-1, m1->h-1);
    size_t i, j, m, n, c;
    if (m1->w == 2)
    {
	double bs = m1->values[0][0] * m1->values[1][1] - m1->values[0][1] * m1->values[1][0];

        return bs;
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
//                  b->values[i][j] = 0;
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
            det = det + s * (m1->values[c][0] * determinant(b));
	    s = -1 * s;
        }
    }

    return (det);
}

Matrix* cofactor(Matrix* m1)
{
    Matrix* b = new_Matrix(m1->w-1, m1->h-1);
    Matrix* fac = new_Matrix(m1->w, m1->h);
    size_t  p, q, m, n, i, j;
    for (q = 0; q < m1->w; q++)
    {
        for (p = 0; p < m1->h; p++)
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
	printf("am transposing :)\n");
    size_t i, j;
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
    return inverse;
}

Matrix* inverse(Matrix* m){
	size_t k,i,j;
	double temp;
	Matrix* I = new_Matrix(m->w,m->h);
	for(size_t t = 0; t<m->w;t++){
		I->values[t][t] = 1;
	}
	for(k=0;k<m->w;k++)
	{ 
        	temp=m->values[k][k];
        	for(j=0;j<m->w;j++)
        	{
        	    m->values[k][j]/=temp;
        	    I->values[k][j]/=temp;
        	}                 
		for(i=0;i<m->w;i++)
		{
		    temp=m->values[i][k];
		    for(j=0;j<m->w;j++) 
		    {
		        if(i==k)
		            break;
		        m->values[i][j]-=m->values[k][j]*temp;
		        I->values[i][j]-=I->values[k][j]*temp;
		    }
		}
        }
	return I;
}
/*
int main()
{
    double d;
    printf("Enter the order of the Matrix : ");
    Matrix* a = new_Matrix(4,4);
//    printf("Enter the elements of %.0fX%.0f Matrix : \n", k, k);
    double** mat = a->values;
    mat[0][0] = 1;
    mat[0][1] = 9;
    mat[0][2] = 3;
    mat[0][3] = -3;
    mat[1][0] = 1;
    mat[1][1] = -5;
    mat[1][2] = 2;
    mat[1][3] = 4;
    mat[2][0] = 1;
    mat[2][1] = 2;
    mat[2][2] = 7;
    mat[2][3] = 4;
    mat[3][0] = 3;
    mat[3][1] = 5;
    mat[3][2] = -6;
    mat[3][3] = 8;
 //print_m(a);
    print_m(a);
    d = determinant(a);
    printf("determinant : %f\n", d);
    if (d == 0)
        printf("\nInverse of Entered Matrix is not possible\n");
    else{
        print_m(cofactor(a));
    	print_m(inverse(a));
    }
}
*/
