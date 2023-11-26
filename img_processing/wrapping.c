#include "wrapping.h"

double FindAngle(Quadrilateral* quad) {
	Point p1 = quad->p1;
	Point p4 = quad->p4;
	double adjacent = (double)(abs(p1.y-p4.y));
	double hypothenuse = sqrt((p1.x - p4.x) * (p1.x - p4.x) + (adjacent*adjacent));
	return acos(adjacent/hypothenuse)* 180. / M_PI;
}

SDL_Surface* Wrapping(char* path, Quadrilateral* quad) {
	double angle = FindAngle(quad);
	printf("Angle is : %f\n",angle);
	SDL_Surface* surface = IMGA_Rotate_from(path,angle);
	SDL_Surface* wrapped = SDL_CreateRGBSurface(0,quad->p2.x-quad->p1.x,quad->p4.y-quad->p1.y,32,0,0,0,0);
	Uint32* newPixels = wrapped->pixels;
	Uint32* pixels = surface->pixels;
	int i = 0;
	for(int h = quad->p1.y; h<quad->p4.y; h++) {
		for(int w = quad->p1.x;w<quad->p2.x;w++) {
			newPixels[i] = pixels[h*surface->w+w];
			i++;
		}
	}
	return wrapped;
}
