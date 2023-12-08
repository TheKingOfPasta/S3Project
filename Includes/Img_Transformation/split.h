#pragma once
#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

SDL_Surface* DigitExtraction(SDL_Surface* s);
void Split(SDL_Surface* surface, char* folder_out);