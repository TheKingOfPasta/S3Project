#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Surface* IMGA_Rotate(SDL_Surface* surface, double angle);

SDL_Surface* IMGA_Rotate_from(char* path, const double angle);

Uint32 GetPixel(SDL_Surface* surface, int x, int y);

double FindSkew(SDL_Surface* surface);

double GetSkewFromFile(char* path);

SDL_Surface* IMGA_Rotate_auto(char* path);
