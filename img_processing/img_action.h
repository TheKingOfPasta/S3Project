#pragma once
#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include "gaussian_blur.h"
#include "adaptive_thresholding.h"
#include "rotate.h"
#include "erosion.h"
#include "canny_edge_detector.h"
#include "img_color.h"
#include "invert_colors.h"
#include "../splitting/split.h"
#include "../hough/utils.h"
#include "../hough/hough.h"
#include "../hough/visualization.h"
#include "../hough/square_detection.h"
#include "wrapping.h"
#include "../hough/grid_detection.h"

#define Blursize 13
#define BlurIntensity 3
#define AdaptiveThreshold 3
#define Splitsize 75000

int CompareStrings(char* a, char* b);

SDL_Surface* IMGA_Rotate(SDL_Surface* surface, double angle);

SDL_Surface* IMGA_Rotate_from(char* path, const double angle);

Uint32 GetPixel(SDL_Surface* surface, int x, int y);

double FindSkew(SDL_Surface* surface);

double GetSkewFromFile(char* path);

SDL_Surface* IMGA_Rotate_auto(char* path);
