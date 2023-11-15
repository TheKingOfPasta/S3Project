#include "wrapping.h"

double FindAngle(Quadrilateral* quad) {
	Point p1 = quad->p1;
	Point p2 = quad->p2;
	double adjacent = (double)(abs(p1.y-p2.y));
	double hypothenuse = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (adjacent*adjacent));
	return acos(adjacent/hypothenuse)* 180. / M_PI;
}

SDL_Surface* Wrapping(char* path, Quadrilateral* quad) {
	double angle = FindAngle(quad);
	printf("Angle is : %f\n\n",angle);
	printf("Attempting to rotate image\n");
	SDL_Surface* wrapped = IMGA_Rotate_from(path,angle);
	printf("Rotated the image\n\n");

	Point p1 = quad->p1;
	Point p2 = quad->p2;
	Point p4 = quad->p4;

	p1.x = p1.x * cos(angle) - p1.y * sin(angle);
	p1.y = p1.y * cos(angle) + p1.x * sin(angle);

	p2.x = p2.x * cos(angle) - p2.y * sin(angle);
	p2.y = p2.y * cos(angle) + p2.x * sin(angle);

	p4.x = p4.x * cos(angle) - p4.y * sin(angle);
	p4.y = p4.y * cos(angle) + p4.x * sin(angle);

	SDL_Rect* rect = malloc(sizeof(SDL_Rect));
	rect->x = p1.x;
	rect->y = p1.y;
	rect->w = abs(p4.x - p1.x);
	rect->h = abs(p1.y - p2.y);
	SDL_Surface* fond_init=SDL_CreateRGBSurface(0, abs(p4.x-p1.x), abs(p1.y-p2.y), 32, 0, 0, 0, 0);
	if (SDL_BlitSurface(wrapped,&rect,fond_init,NULL) != 0)
		errx(1,"Can't blit the Surface\n");
	return fond_init;
}

SDL_Surface* Wrapping_Copy(char* path, Quadrilateral* quad) {
	double angle = FindAngle(quad);
	printf("Angle is : %f\n",angle);
	SDL_Surface* surface = IMGA_Rotate_from(path,angle);
	SDL_Surface* wrapped = SDL_CreateRGBSurface(0,quad->p4.x-quad->p1.x,quad->p2.y-quad->p1.y,32,0,0,0,0);
	Uint32* newPixels = wrapped->pixels;
	Uint32* pixels = surface->pixels;
	int i = 0;
	for(int h = quad->p1.y; h<quad->p2.y; h++) {
		for(int w = quad->p1.x;w<quad->p4.x;w++) {
			newPixels[i] = pixels[h*surface->w+w];
			i++;
		}
	}
	return wrapped;
}
