#pragma once
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "utils.h"
#include "visualization.h"

List* HoughLine(SDL_Surface* img);
void Prune(List* lLine);
void ExcludeBorder(List* list, int w, int h, double exthres);
void LineFiltering(List *l,int thresholdDeg);