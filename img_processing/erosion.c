#include "erosion.h"

SDL_Surface* IMGA_Erosion(SDL_Surface* input){
	SDL_Surface* output =
	   	SDL_CreateRGBSurface(0,input->w,input->h,32,0,0,0,0);

    SDL_LockSurface(output);

    Uint32* inpPixels = (Uint32*)(input->pixels);
    Uint32* outPixels = (Uint32*)(output->pixels);

	int sumWhite;
	for (int i = 1; i < input->w-1; i++)
	for (int j = 1; j < input->h-1; j++)
	{
        sumWhite =0;
		for (int k =-1; k <= 1; k++)
		for (int l =-1; l <= 1; l++)
		{
            if (inpPixels[i + k + (j + l) * input->w])
                sumWhite++;
		}

		Uint32* outpxl = (Uint32*)outPixels + j*output->w + i;

		*outpxl = ( sumWhite< 4) ?
            SDL_MapRGB(output->format,   0,   0,   0) :
            SDL_MapRGB(output->format, 255, 255, 255);
	}

	SDL_UnlockSurface(output);
	SDL_FreeSurface(input);
	return output;
}