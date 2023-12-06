#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* IMGA_Erosion(SDL_Surface* input);
SDL_Surface* IMGA_Erode(SDL_Surface* s, int size);
SDL_Surface* IMGA_Dilate(SDL_Surface* s, int size);
