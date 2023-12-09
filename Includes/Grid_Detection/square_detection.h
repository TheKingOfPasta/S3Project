#pragma once
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "utils.h"
#include "visualization.h"

List* FindSquares(List* lHor, List* lVer,int width, int height,
                  double paddingPercentage);
Quadrilateral* BestSquare(List * lQuad);