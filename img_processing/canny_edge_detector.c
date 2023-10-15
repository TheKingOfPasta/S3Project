#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <math.h>

SDL_Surface* sobel_gradient(SDL_Surface input,int threshold){
	SDL_Surface output =
	   	SDL_CreateRGBSurface(0,input->w,input->h,32,0,0,0,0 );

    SDL_LockSurface(output);

    Uint32* inpPixels = (Uint32*)(inp->pixels);
    Uint32* outPixels = (Uint32*)(out->pixels);

	int ker[3][3]= {
		{ 1, 2, 1},
		{ 0, 0, 0},
		{-1,-2,-1},
	}

	for (size_t i = 1;i<input->w-1;i++)
	for (size_t j = 1;j<input->h-1;j++){
		int Gx =0,Gy =0;
		for (size_t k =-1;k<2;k++)
		for (size_t l =-1;l<2;l++){
			Uint8* inpxl = (Uint8*)inpPixels+ (j+k)*input->pitch + i+l;
			gx += ker[k+1][l+1]*inpxl;
			gy += ker[l+1][k+1]*inpxl;
		}
		Uint8* outpxl = (Uint8*)outPixels + j*output->pitch + i;
		*outpxl = ( sqrt(gx*gx+gy*gy)>threshold)? 255 : 0;
	}
	SDL_UnlockSurface(output);
	return output;
}
