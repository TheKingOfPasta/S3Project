#include <math.h>
#include <stdio.h>
#include "../Includes/Utils/matrix.h"

double determinant(Matrix* m);

Matrix* cofactor(Matrix* m);

Matrix* transpose(Matrix* m1, Matrix* m2);

Matrix* inverse(Matrix* m);
