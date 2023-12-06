#include "wrapping.h"

double FindAngle(Quadrilateral* quad) {
	Point p1 = quad->p1;
	Point p4 = quad->p4;
	Point p2 = quad->p2;
	Point p3 = quad->p3;
	double adjacent = (double)(abs(p1.y-p4.y));
	double hypothenuse = sqrt((p1.x - p4.x) * (p1.x - p4.x) + (adjacent*adjacent));
	double angle = acos(adjacent / hypothenuse) * 180. / M_PI;

	double cosine = cos(angle);
	double sine = sin(angle);

	p1.x = (int)(p1.x * cosine - p1.y * sine);
	p1.y = (int)(p1.y * cosine + p1.x * sine);
	p2.x = (int)(p2.x * cosine - p2.y * sine);
	p2.y = (int)(p2.y * cosine + p2.x * sine);
	p3.x = (int)(p3.x * cosine - p3.y * sine);
	p3.y = (int)(p3.y * cosine + p3.x * sine);
	p4.x = (int)(p4.x * cosine - p4.y * sine);
	p4.y = (int)(p4.y * cosine + p4.x * sine);

	return angle;
}

SDL_Surface* Wrapping(char* path, Quadrilateral* quad) {
	double angle = FindAngle(quad);
	printf("Angle is : %f\n",angle);
	SDL_Surface* surface = IMGA_Rotate_from(path,angle);
	IMG_SavePNG(surface, "Rotated.png");
	printf("Rotated the image :)\n");
	SDL_Surface* wrapped = SDL_CreateRGBSurface(0,quad->p4.x-quad->p1.x,quad->p2.y-quad->p1.y,32,0,0,0,0);
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

SDL_Surface* WrappingSurface(SDL_Surface* sur, Quadrilateral* quad) {
	SDL_Surface* wrapped = SDL_CreateRGBSurface(0, quad->p2.x - quad->p1.x, quad->p4.y - quad->p1.y, 32, 0, 0, 0, 0);
	Uint32* newPixels = wrapped->pixels;
	Uint32* pixels = sur->pixels;
	int i = 0;
	for (int h = quad->p1.y; h < quad->p4.y; h++) {
		for (int w = quad->p1.x; w < quad->p2.x; w++) {
			newPixels[i] = pixels[h * sur->w + w];
			i++;
		}
	}
	return wrapped;
}
