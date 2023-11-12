#include "resize.h"

SDL_Surface* Resize(SDL_Surface* surface, int newSize) {
	SDL_Surface* resize = SDL_CreateRGBSurface(0,newSize,newSize,32,0,0,0,0);
	double scale = surface->w / newSize;
	Uint32* pixels = resize->pixels;
	Uint32* p = surface->pixels;
	for (int x =0; x<newSize; x++) {
		for (int y = 0; y<newSize; y++) {
			pixels[x+y*newSize] = p[(int)(x*scale+(y*scale)*surface->w)];
		}
	}
	return resize;
}

int main(int argc, char** argv) {
	if (argc != 3)
		errx(1,"input -> output");
	SDL_Surface* resize = IMG_Load(argv[1]);
	IMG_SavePNG(Resize(resize,120),argv[2]);
	return 0;
}
