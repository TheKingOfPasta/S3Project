#pragma once
#include "utils.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>

void Visualize_Acc(unsigned int acc[][180] , int rmax, int maxacc);
SDL_Surface* ListToSurface(SDL_Surface* s,ListLine* list,int r,int g,int b);
void printList(ListLine* l);
void DrawSquare(SDL_Surface* s, NodeQuadrilateral *q);
void printQuad(NodeQuadrilateral* q);