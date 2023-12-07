#pragma once
#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include "gaussian_blur.h"
#include "adaptive_thresholding.h"
#include "filter.h"
#include "canny_edge_detector.h"
#include "img_color.h"
#include "../Img_Transformation/rotate.h"
#include "../Img_Transformation/wrapping.h"
#include "../Img_Transformation/resize.h"
#include "../Img_Transformation/split.h"
#include "../Grid_Detection/utils.h"
#include "../Grid_Detection/hough.h"
#include "../Grid_Detection/visualization.h"
#include "../Grid_Detection/grid_detection.h"

#define Blursize 13
#define BlurIntensity 3
#define AdaptiveThreshold 3
#define Splitsize 75000

int CompareStrings(char* a, char* b);