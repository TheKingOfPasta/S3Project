#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* IMGA_GaussianBlur(SDL_Surface* surface, int size, double sigma);
