#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Surface* IMGA_OtsuThreshold(SDL_Surface* surface);
SDL_Surface* IMGA_Sauvola(SDL_Surface *s,int n, double k);
SDL_Surface* IMGA_ApplyThreshold(SDL_Surface* surface, int threshold,  int splitsize);
