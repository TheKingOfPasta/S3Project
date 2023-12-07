#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <math.h>
#include "../hough/utils.h"
#include "rotate.h"

double FindAngle(Quadrilateral* quad,int width, int height);

SDL_Surface* Wrapping(char* path, Quadrilateral* quad, int width, int height);

SDL_Surface* WrappingSurface(SDL_Surface* sur, Quadrilateral* quad);