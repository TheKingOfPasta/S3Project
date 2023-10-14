#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct button
{
	int x;
	int y;

	int height;
	int width;

	char* text;

	void (*func)();//The function which will be called on click
} button;


button Button(int x, int y, int width, int height, char* text, void (*func)())//Constuctor
{
	if (func == NULL)
		errx(EXIT_FAILURE, "Creating button without function associated");
	
	button b;
	b.x = x;
	b.y = y;
	b.height = height;
	b.width = width;
	b.text = text;
	b.func = func;

	return b;
}
