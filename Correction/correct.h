#include <math.h>
#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Matrix* Inverse(Matrix* m);

Matrix* CorrectMatrix(Matrix* m1, Matrix* m2);

SDL_Surface* CorrectImage(SDL_Surface* surface, Quadrilateral* grid);