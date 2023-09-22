#pragma once

void PSR_parse(char* s, short grid[9][9]);
char* PSR_unparse(short grid[9][9]);
void PSR_parse_file(char* file, short grid[9][9]);