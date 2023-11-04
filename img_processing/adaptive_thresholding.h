#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* IMGA_ApplyThreshold(SDL_Surface* surface, int threshold,  int splitsize);
