#pragma once
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "utils.h"
#include "visualization.h"
#include "../Img_Processing/filter.h"

void AveragesCloseLine(List* lLine, int diag_len,int angleThreshold, double rhoThreshold);
void ExcludeBorderLine(List* list, int w, int h, double exthres);
void RemovesStrayLine(List *l,int thresholdDeg, List* ver, List* hor);
void RemoveFalseLines(List* l , SDL_Surface* surface, int dilateStrength, int segmentThreshold);