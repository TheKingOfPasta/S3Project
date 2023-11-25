#pragma once
#define _GNU_SOURCE

#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/stat.h>
#include "../img_processing/gaussian_blur.h"
#include "../img_processing/adaptive_thresholding.h"
#include "../img_processing/rotate.h"
#include "../img_processing/canny_edge_detector.h"
#include "../img_processing/img_color.h"
#include "../img_processing/invert_colors.h"
#include "../hough/utils.h"
#include "../hough/hough.h"
#include "../hough/visualization.h"
#include "../hough/square_detection.h"
#include "../img_processing/erosion.h"
#include "../hough/grid_detection.h"
#include "../img_processing/wrapping.h"
#include "../splitting/split.h"
#include "../solver/solver.h"