#pragma once
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "utils.h"

ListQuad* FindSquares(ListLine* l,int width, int height);
NodeQuadrilateral* BestSquare(ListQuad *l);