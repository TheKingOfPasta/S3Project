#pragma once
#include "utils.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <math.h>

void Visualize_Acc( int **acc , int rmax, int maxacc);
SDL_Surface* DrawLines(SDL_Surface* s,List* list,int r,int g,int b);
void printLine(Line* l);
void printList(List* l, int line);
void DrawSquare(SDL_Surface* s, Quadrilateral *q,int r,int g,int b);
void printQuad(Quadrilateral* q);
void DrawIntersections(SDL_Surface* s,List* list);