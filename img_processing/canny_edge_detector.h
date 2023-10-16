#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* sobel_gradient(SDL_Surface* input, int threshold);
