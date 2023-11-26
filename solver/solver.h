#pragma once
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>

void SLV_collapse_grid(short grid[9][9], int x, int y, short val);
short SLV_get_collapsed_value(short cell_val);
void printUnsolved(short x);
short SLV_rmv_possibility(short* cell_val, short possibility);
void SLV_collapse_cell(short grid[9][9],int x,int y,short val);
void SLV_Clues_Collapsing(short grid[9][9]);
int SLV_is_Placeable(short g[9][9], int x, int y, short val);
void SLV_backtracking(short grid[9][9]);
void SLV_findRemainingCells(short g[9][9],short a[]);
void SLV_solve(short grid[9][9]);
