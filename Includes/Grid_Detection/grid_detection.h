#pragma once
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "utils.h"
#include "visualization.h"
#include "hough.h"
#include "square_detection.h"
#include "line_filtering.h"

Quadrilateral* Find_Grid(SDL_Surface *s );