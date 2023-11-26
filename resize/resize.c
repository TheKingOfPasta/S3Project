#include "resize.h"

SDL_Surface* Resize(SDL_Surface* surface, int newSize) {
	SDL_Surface* resize = SDL_CreateRGBSurface(0,newSize,newSize,32,0,0,0,0);
	double scale = surface->w / (double)newSize;
	Uint32* pixels = resize->pixels;
	Uint32* p = surface->pixels;
	for (int x =0; x<newSize; x++) {
		for (int y = 0; y<newSize; y++) {
			pixels[x+y*newSize] = p[(int)(x*scale+(y*scale)*surface->w)];
		}
	}
	return resize;
}

SDL_Surface* Resample(SDL_Surface* surface, int newSize) {
	SDL_Surface* resample = SDL_CreateRGBSurface(0,newSize,newSize,32,0,0,0,0);
	double scale = (double)newSize/(double)surface->w;
	Uint32* pixels = resample->pixels;
	Uint32* p = surface->pixels;
	for (int cy = 0; cy<newSize;cy++) {
		for(int cx = 0; cx<newSize;cx++){
			int pixel = (cy*(newSize)) + (cx);
			int nearestMatch = (((int)(cy/scale) * (surface->w)) + ((int)(cx/scale)));
			//printf("pixel: %i\nnearestMatch: %i\n",pixel,nearestMatch);
			pixels[pixel] = p[nearestMatch];
		}
	}
	return resample;
}

int toInt(char* c) {
	int total = 0;
	for(int i = 0; c[i];i++) {
		total*=10;
		total += c[i]-'0';
	}
	return total;
}

int main(int argc, char** argv) {
	if (argc != 4)
		errx(1,"use : ./resize [input path] [output path] [size to resize to]\n");
	SDL_Surface* resize = IMG_Load(argv[1]);
	IMG_SavePNG(Resample(resize,toInt(argv[3])),argv[2]);
	SDL_FreeSurface(resize);
	return 0;
}
