#include "Utils/inverse.h"

/*For calculating Determinant of the Matrix */
/*double determinant(Matrix* m1)
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
}*/
double determinant(Matrix* mat)
{
    if (mat->w != mat->h) {
        errx(EXIT_FAILURE,"Error: Input matrix must be square.\n");
    }
    if (mat->w == 1) {
        return mat->values[0][0];
    } else if (mat->w == 2) {
        return mat->values[0][0] * mat->values[1][1] - mat->values[0][1] *
               mat->values[1][0];
    } else {
        double det = 0.0;
        for (size_t i = 0; i < mat->w; ++i) {
            // Calculate the cofactor
            Matrix* subMat = new_Matrix(mat->w - 1, mat->h - 1);
            for (size_t j = 1; j < mat->h; ++j) {
                for (size_t k = 0; k < i; ++k) {
                    subMat->values[j - 1][k] = mat->values[j][k];
                }
                for (size_t k = i + 1; k < mat->w; ++k) {
                    subMat->values[j - 1][k - 1] = mat->values[j][k];
                }
            }
            // Recursive call to compute the determinant
            det += (i % 2 == 0 ? 1 : -1) * mat->values[0][i] * determinant(subMat);
            // Free memory for the submatrix
            free_m(subMat);
        }
        return det;
    }
}

Matrix* cofactor(Matrix* m1)
{
    Matrix* b = new_Matrix(m1->w-1, m1->h-1);
    Matrix* fac = new_Matrix(m1->w, m1->h);
    size_t  p, q, m, n, i, j;
    for (q = 0; q < m1->w; q++) {
        for (p = 0; p < m1->h; p++) {
            m = 0;
            n = 0;
            for (i = 0; i < m1->w; i++) {
                for (j = 0; j < m1->w; j++) {
                    if (i != q && j != p) {
                        b->values[m][n] = m1->values[i][j];
                        if (n < (m1->w - 2))
                            n++;
                        else {
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
    for (i = 0; i < m1->w; i++) {
        for (j = 0; j < m1->w; j++) {
            b->values[i][j] = m2->values[j][i];
        }
    }
    d = determinant(m1);
    for (i = 0; i < m1->w; i++) {
        for (j = 0; j < m1->w; j++) {
            inverse->values[i][j] = b->values[i][j] / d;
        }
    }
    printf("\n\n\nThe inverse of matrix is : \n");
    return inverse;
}
/*
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
	//free_m(m);
	return I;
}
*/
// Function to swap two rows of a matrix
void swapRows(Matrix *mat, size_t row1, size_t row2)
{
    double *temp = mat->values[row1];
    mat->values[row1] = mat->values[row2];
    mat->values[row2] = temp;
}

// Function to perform Gaussian elimination to get an upper triangular matrix
void gaussianElimination(Matrix *mat, Matrix *identity)
{
    size_t size = mat->w;
    for (size_t i = 0; i < size; ++i) {
        // Find the pivot row
        size_t pivot = i;
        for (size_t j = i + 1; j < size; ++j) {
            if (fabs(mat->values[j][i]) > fabs(mat->values[pivot][i])) {
                pivot = j;
            }
        }
        // Swap rows to make the pivot the maximum element
        swapRows(mat, i, pivot);
        swapRows(identity, i, pivot);
        // Make the diagonal element 1
        double pivotValue = mat->values[i][i];
        for (size_t j = 0; j < size; ++j) {
            mat->values[i][j] /= pivotValue;
            identity->values[i][j] /= pivotValue;
        }
        // Eliminate other elements in the column
        for (size_t j = 0; j < size; ++j) {
            if (j != i) {
                double factor = mat->values[j][i];
                for (size_t k = 0; k < size; ++k) {
                    mat->values[j][k] -= factor * mat->values[i][k];
                    identity->values[j][k] -= factor * identity->values[i][k];
                }
            }
        }
    }
}

// Function to compute the inverse of a matrix
Matrix* inverse(Matrix* mat)
{
    if (mat->w != mat->h) {
        errx(EXIT_FAILURE,"Error: Input matrix must be square.\n");
    }
    size_t size = mat->w;
    // Create a copy of the input matrix and an identity matrix
    Matrix* copy = new_Matrix(size, size);
    Matrix* identity = new_Matrix(size,size);
    for(size_t i = 0; i<size; i++) {
        identity->values[i][i] = 1;
    }
    //print_m(identity);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            copy->values[i][j] = mat->values[i][j];
        }
    }
    // Perform Gaussian elimination to get an upper triangular matrix
    gaussianElimination(copy, identity);
    // Free memory for the copy matrix
    free_m(copy);
    return identity;
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
