#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <math.h>

double FindAngle(Quadrilateral* quad);

SDL_Surface* Wrapping(char* path, Quadrilateral* quad);

SDL_Surface* Wrapping_Copy(char* path, Quadrilateral* quad);
