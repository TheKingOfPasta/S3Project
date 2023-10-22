#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct button
{
	int x;
	int y;

	int height;
	int width;

	char* text;

	char* func;//The function which will be called on click
} button;


button Button(int x, int y, char* text, char* func)//Constuctor
{	
	button b;
	b.x = x;
	b.y = y;
	b.height = 10;
	b.width = 1 + 2 * strlen(text) * 16 / 9;
	b.text = text;
	b.func = func;

	return b;
}
