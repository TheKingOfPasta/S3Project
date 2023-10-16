#include <err.h>
#include <stdio.h>
#include <math.h>
#include "canny_edge_detector.h"

SDL_Surface* sobel_gradient(SDL_Surface* input,int threshold){
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

	for (int i = 1; i < input->w-1; i++)
	for (int j = 1; j < input->h-1; j++)
	{
		int gx = 0;
		int gy = 0;

		for (int k =-1; k <= 1; k++)
		for (int l =-1; l <= 1; l++)
		{
			Uint32* inpxl = (Uint32*)inpPixels + (j+k)*input->w + i+l;

			gx += ker[k+1][l+1]*(*inpxl);
			gy += ker[l+1][k+1]*(*inpxl);
		}

		Uint32* outpxl = (Uint32*)outPixels + j*output->w + i;

		*outpxl = ( sqrt(gx*gx+gy*gy) > threshold) ? SDL_MapRGB(output->format, 255, 255, 255) : 0;
	}

	SDL_UnlockSurface(output);
	
	return output;
}
