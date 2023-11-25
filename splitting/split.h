#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

typedef struct tuple
{
    int x;
    int y;
} tuple;

void Split(SDL_Surface* surface, char* folder_out, tuple* coords);