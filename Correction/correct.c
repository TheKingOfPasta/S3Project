#include "correct.h"

Matrix* CorrectMatrix(Matrix* m1, Matrix* m2) {
	Matrix* P = new_Matrix(9, 9);
    P->values = {
        {-m1->values[0][0], -m1->values[0][1], -1, 0, 0, 0, m1->values[0][0] * m2->values[0][0],
            m1->values[0][1] * m2->values[0][0], m2->values[0][0]},
        {0, 0, 0, -m1->values[0][0], -m1->values[0][1], -1, m1->values[0][0] * m2->values[0][1],
            m1->values[0][1] * m2->values[0][1], m2->values[0][1]},
        {-m1->values[1][0], -m1->values[1][1], -1, 0, 0, 0, m1->values[1][0] * m2->values[1][0],
            m1->values[1][1] * m2->values[1][0], m2->values[1][0]},
        {0, 0, 0, -m1->values[1][0], -m1->values[1][1], -1, m1->values[1][0] * m2->values[1][1],
            m1->values[1][1] * m2->values[1][1], m2->values[1][1]},
        {-m1->values[2][0], -m1->values[2][1], -1, 0, 0, 0, m1->values[2][0] * m2->values[2][0],
            m1->values[2][1] * m2->values[2][0], m2->values[2][0]},
        {0, 0, 0, -m1->values[2][0], -m1->values[2][1], -1, m1->values[2][0] * m2->values[2][1],
            m1->values[2][1] * m2->values[2][1], m2->values[2][1]},
        {-m1->values[3][0], -m1->values[3][1], -1, 0, 0, 0, m1->values[3][0] * m2->values[3][0],
            m1->values[3][1] * m2->values[3][0], m2->values[3][0]},
        {0, 0, 0, -m1->values[3][0], -m1->values[3][1], -1, m1->values[3][0] * m2->values[3][1],
            m1->values[3][1] * m2->values[3][1], m2->values[3][1]},
        {0, 0, 0, 0, 0, 0, 0, 0, 1}
    };
    Matrix* R = new_Matrix(1, 9);
    R->values = { 0,0,0,0,0,0,0,0,1 };
	
	return m3;
}

SDL_Surface* CorrectImage(SDL_Surface* surface, Quadrilateral* grid) {


	SDL_Surface* sur = SDL_CreateRGBSurface(0, , , 32, 0, 0, 0, 0);

	return sur;
}