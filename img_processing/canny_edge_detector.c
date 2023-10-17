#include <err.h>
#include <stdio.h>
#include <math.h>
#include "canny_edge_detector.h"
#define WHITE SDL_MapRGB(output->format, 255, 255, 255)
#define BLACK SDL_MapRGB(output->format,   0,   0,   0)

SDL_Surface* sobel_gradient(SDL_Surface* input)
{
	SDL_Surface* output =
	   	SDL_CreateRGBSurface(0,input->w,input->h,32,0,0,0,0);

    SDL_LockSurface(output);

    Uint32* inpPixels = (Uint32*)(input->pixels);
    Uint32* outPixels = (Uint32*)(output->pixels);

	int ker[3][3]= {
		{ 1, 2, 1},
		{ 0, 0, 0},
		{-1,-2,-1},
	};
	Uint8 r,g,b;
	for (int i = 1; i < input->w-1; i++)
	for (int j = 1; j < input->h-1; j++)
	{
		int gx = 0;
		int gy = 0;

		for (int k =-1; k <= 1; k++)
		for (int l =-1; l <= 1; l++)
		{
			//Uint32* inpxl = (Uint32*)inpPixels + (j+k)*input->w + i+l;
                SDL_GetRGB(
                    inpPixels[i + k + (j + l) * input->w],
                    input->format,
                    &r,
                    &g,
                    &b);
			gx += ker[k+1][l+1]*r;
			gy += ker[l+1][k+1]*r;
		}

		Uint32* outpxl = (Uint32*)outPixels + j*output->w + i;

		*outpxl = ( sqrt(gy*gy+gx*gx) > 127) ? WHITE : BLACK;
	}

	SDL_UnlockSurface(output);
	SDL_FreeSurface(input);
	return output;
}
