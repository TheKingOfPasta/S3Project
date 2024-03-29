#include "Img_Transformation/correct.h"

Matrix* inv(Matrix* M)
{
    Matrix* m = new_Matrix(3,3);
    double** val = m->values;
    double MM = M->values[0][0] * M->values[1][1] * M->values[2][2] +
                M->values[0][1] * M->values[1][2] * M->values[2][0]
                + M->values[0][2] * M->values[2][1] * M->values[1][0] - M->values[0][2] *
                M->values[1][1] * M->values[2][0]
                - M->values[0][1] * M->values[1][0] * M->values[2][2] - M->values[0][0] *
                M->values[2][1] * M->values[1][2];
    val[0][0] = M->values[1][1] * M->values[2][2] - M->values[1][2] *
                M->values[2][1];
    val[0][1] = M->values[0][2] * M->values[2][1] - M->values[0][1] *
                M->values[2][2];
    val[0][2] = M->values[0][1] * M->values[1][2] - M->values[0][2] *
                M->values[1][1];
    val[1][0] = M->values[1][2] * M->values[2][0] - M->values[1][0] *
                M->values[2][2];
    val[1][1] = M->values[0][0] * M->values[2][2] - M->values[0][2] *
                M->values[2][0];
    val[1][2] = M->values[0][2] * M->values[1][0] - M->values[0][0] *
                M->values[1][2];
    val[2][0] = M->values[1][0] * M->values[2][1] - M->values[1][1] *
                M->values[2][0];
    val[2][1] = M->values[0][1] * M->values[2][0] - M->values[0][0] *
                M->values[2][1];
    val[2][2] = M->values[0][0] * M->values[1][1] - M->values[0][1] *
                M->values[1][0];
    Matrix* m2 = new_Matrix(3,3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m2->values[i][j] = m->values[i][j] / MM;
    free_m(m);
    return m2;
}


Matrix* mult(Matrix* m1, Matrix* m2)
{
    //m2 is a line matrix
    Matrix* m = new_Matrix(1,m2->h);
    for(size_t i = 0; i<m2->h; i++) {
        for(size_t j = 0; j<m2->h; j++) {
            m->values[0][i] += m1->values[i][j] * m2->values[0][j];
        }
    }
    return m;
}

Matrix* CorrectMatrix(Matrix* m1, Matrix* m2)
{
    Matrix* P = new_Matrix(9, 9);
    double** val = P->values;
    val[0][0] = -m1->values[0][0];
    val[1][0] = -m1->values[0][1];
    val[2][0] = -1;
    val[3][0] = 0;
    val[4][0] = 0;
    val[5][0] = 0;
    val[6][0] = m1->values[0][0]*m2->values[0][0];
    val[7][0] = m1->values[0][1]*m2->values[0][0];
    val[8][0] = m2->values[0][0];
    val[0][1] = 0;
    val[1][1] = 0;
    val[2][1] = 0;
    val[3][1] = -m1->values[0][0];
    val[4][1] = -m1->values[0][1];
    val[6][1] = m1->values[0][0]*m2->values[0][1];
    val[7][1] = m1->values[0][1]*m2->values[0][1];
    val[5][1] = -1;
    val[8][1] = m2->values[0][1];
    val[0][2] = -m1->values[1][0];
    val[1][2] = -m1->values[1][1];
    val[2][2] = -1;
    val[3][2] = 0;
    val[4][2] = 0;
    val[5][2] = 0;
    val[6][2] = m1->values[1][0]*m2->values[1][0];
    val[7][2] = m1->values[1][1]*m2->values[1][0];
    val[8][2] = m2->values[1][0];
    val[0][3] = 0;
    val[1][3] = 0;
    val[2][3] = 0;
    val[3][3] = -m1->values[1][0];
    val[4][3] = -m1->values[1][1];
    val[6][3] = m1->values[1][0]*m2->values[1][1];
    val[7][3] = m1->values[1][1]*m2->values[1][1];
    val[5][3] = -1;
    val[8][3] = m2->values[1][1];
    val[0][4] = -m1->values[2][0];
    val[1][4] = -m1->values[2][1];
    val[2][4] = -1;
    val[3][4] = 0;
    val[4][4] = 0;
    val[5][4] = 0;
    val[6][4] = m1->values[2][0] * m2->values[2][0];
    val[7][4] = m1->values[2][1] * m2->values[2][0];
    val[8][4] = m2->values[2][0];
    val[0][5] = 0;
    val[1][5] = 0;
    val[2][5] = 0;
    val[3][5] = -m1->values[2][0];
    val[4][5] = -m1->values[2][1];
    val[6][5] = m1->values[2][0] * m2->values[2][1];
    val[7][5] = m1->values[2][1] * m2->values[2][1];
    val[5][5] = -1;
    val[8][5] = m2->values[2][1];
    val[0][6] = -m1->values[3][0];
    val[1][6] = -m1->values[3][1];
    val[2][6] = -1;
    val[3][6] = 0;
    val[4][6] = 0;
    val[5][6] = 0;
    val[6][6] = m1->values[3][0] * m2->values[3][0];
    val[7][6] = m1->values[3][1] * m2->values[3][0];
    val[8][6] = m2->values[3][0];
    val[0][7] = 0;
    val[1][7] = 0;
    val[2][7] = 0;
    val[3][7] = -m1->values[3][0];
    val[4][7] = -m1->values[3][1];
    val[6][7] = m1->values[3][0] * m2->values[3][1];
    val[7][7] = m1->values[3][1] * m2->values[3][1];
    val[5][7] = -1;
    val[8][7] = m2->values[3][1];
    val[8][8] = 1;
    Matrix* R = new_Matrix(1, 9);
    double** va = R->values;
    va[0][8] = 1;
    Matrix* inve = inverse(P);
    Matrix* cor = Multiply_m(inve,R);
    Matrix* lul = new_Matrix(3,3);
    int v = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++, v++) {
            lul->values[i][j] = cor->values[0][v];
        }
    }
    free_m(cor); //no bad Alekushi
    free_m(inve);
    free_m(R);
    free_m(P);
    double d = determinant(lul);
    if (d == d) {
        Matrix* l = inv(lul);
        free_m(lul);
        return l;
    } else {
        free_m(lul);
        return NULL;
    }
}

SDL_Surface* CorrectImage(SDL_Surface* surface, Quadrilateral* grid)
{
    Matrix* s = new_Matrix(4,2);
    double** src = s->values;
    src[0][0] = grid->p1.x;
    src[0][1] = grid->p1.y;
    src[1][0] = grid->p2.x;
    src[1][1] = grid->p2.y;
    src[2][0] = grid->p3.x;
    src[2][1] = grid->p3.y;
    src[3][0] = grid->p4.x;
    src[3][1] = grid->p4.y;
    int e1 = sqrt(pow(src[0][0]-src[1][0],2) + pow(src[0][1]-src[1][1],2));
    int e2 = sqrt(pow(src[1][0]-src[2][0],2) + pow(src[1][1]-src[2][1],2));
    int e3 = sqrt(pow(src[2][0]-src[3][0],2) + pow(src[2][1]-src[3][1],2));
    int e4 = sqrt(pow(src[3][0]-src[0][0],2) + pow(src[3][1]-src[0][1],2));
    double max = fmax(fmax(e4,e2),fmax(e1,e3));
    Matrix* de = new_Matrix(4,2);
    double** d = de->values;
    d[0][0] = 0;
    d[0][1] = 0;
    d[1][0] = max;
    d[1][1] = 0;
    d[2][0] = 0;
    d[2][1] = max;
    d[3][0] = max;
    d[3][1] = max;
    Matrix* inv = CorrectMatrix(s,de);
    if (inv == NULL) {
        free_m(de);
        free_m(s);
        return surface;
    }
    SDL_Surface* sur = SDL_CreateRGBSurface(0, max, max, 32, 0, 0, 0, 0);
    Uint32* pixels = surface->pixels;
    Uint32* p = sur->pixels;
    for(int i = 0; i<(int)max; i++) {
        for(int j = 0; j<(int)max; j++) {
            Matrix* old = new_Matrix(1,3);
            old->values[0][0] = i;
            old->values[0][1] = j;
            old->values[0][2] = 1;
            Matrix* new = mult(inv,old);
            int x = (int)(new->values[0][0]/new->values[0][2]);
            int y = (int)(new->values[0][1]/new->values[0][2]);
            if (x>= 0 && y>= 0 && x<surface->w && y<surface->h) {
                p[(int)(j*max+i)] = pixels[(int)(y*surface->w+x)];
            } else {
                p[(int)(j*max+i)] = SDL_MapRGB(surface->format,255,255,255);
            }
            free_m(old);
            free_m(new);
        }
    }
    grid->p1.x = 0;
    grid->p1.y = 0;
    grid->p2.x = max;
    grid->p2.y = 0;
    grid->p3.x = 0;
    grid->p3.y = max;
    grid->p4.x = max;
    grid->p4.y = max;
    free_m(inv);
    free_m(s);
    free_m(de);
    return sur;
}
