#include "Img_Processing/filter.h"

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

		*outpxl = ( sumWhite< 5) ?
            SDL_MapRGB(output->format,   0,   0,   0) :
            SDL_MapRGB(output->format, 255, 255, 255);
	}

	SDL_UnlockSurface(output);
	SDL_FreeSurface(input);
	return output;
}

SDL_Surface* IMGA_Erode(SDL_Surface* s, int size){
	SDL_Surface* newS =
	   	SDL_CreateRGBSurface(0,s->w,s->h,32,0,0,0,0);
  	SDL_PixelFormat* format = s->format;

    Uint32* inpPixels = s->pixels;
    Uint32* outPixels = newS->pixels;

	if(!size%2) size++;
	Uint8 min,gray;
	int midRange = size>>1;


    SDL_LockSurface(newS);
	for (int i = 0; i < s->w; i++)
	for (int j = 0; j < s->h; j++)
	{
        min=255;
		for (int k =i-midRange; k <=i+midRange; k++)
		for (int l =j-midRange; l <=j+midRange; l++)
		{
			if(k<0 || l<0 || k>=s->w || l>=s->h) continue;
            int offset = l * s->pitch + k * format->BytesPerPixel;
    		SDL_GetRGB(( *(Uint32*)((Uint8*)inpPixels + offset)), format,
      			&gray, &gray, &gray);
			if(min > gray) min = gray;
		}

		outPixels[i+s->w*j] = SDL_MapRGB(newS->format, min, min, min);
	}

	SDL_UnlockSurface(newS);
	SDL_FreeSurface(s);
	return newS;
}

SDL_Surface* IMGA_Dilate(SDL_Surface* s, int size){
	SDL_Surface* newS =
	   	SDL_CreateRGBSurface(0,s->w,s->h,32,0,0,0,0);
  	SDL_PixelFormat* format = s->format;

    Uint32* inpPixels = s->pixels;
    Uint32* outPixels = newS->pixels;

	if(!size%2) size++;
	Uint8 max,gray;
	int midRange = size>>1;


    SDL_LockSurface(newS);
	for (int i = 0; i < s->w; i++)
	for (int j = 0; j < s->h; j++)
	{
        max=0;
		for (int k =i-midRange; k <=i+midRange; k++)
		for (int l =j-midRange; l <=j+midRange; l++)
		{
			if(k<0 || l<0 || k>=s->w || l>=s->h) continue;
            int offset = l * s->pitch + k * format->BytesPerPixel;
			SDL_GetRGB(*(Uint32*)((Uint8*)inpPixels + offset), format,
      			&gray, &gray, &gray);
			if(max < gray) max = gray;
		}

		outPixels[i+s->w*j] = SDL_MapRGB(newS->format, max, max, max);
	}

	SDL_UnlockSurface(newS);
	SDL_FreeSurface(s);
	return newS;
}