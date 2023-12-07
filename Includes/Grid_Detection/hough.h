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
void AveragesCloseLine(List* lLine, int diag);
void ExcludeBorderLine(List* list, int w, int h, double exthres);
void RemovesStrayLine(List *l,int thresholdDeg, List* ver, List* hor);