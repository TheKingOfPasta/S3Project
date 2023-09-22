#pragma once

void printUnsolved(short x);

void wave_function_collapse(short grid[9][9]);

void collapse_grid(short grid[9][9], int x, int y,short val);

void collapse_cell(short grid[9][9],int x,int y,short val);

short collapsed_cell_value(short cell_val);

short rmv_possibility(short* cell_val,short possibility);