#include <err.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
# include <math.h>

/**
  * A better version of the resizes. It better handles small images resizing
  * and avoid pixel lose by making the average of neigbours.
  * Also called Box sampling resizing
  *
  * surface: the surface to resize
  * new_size: the new width and height of the surface
  *
  * Return a new allocated resized surface
  */
SDL_Surface* downscale_resize(SDL_Surface *surface, int new_size);

SDL_Surface* Resize(SDL_Surface* surface, int newSize);

SDL_Surface* Resample(SDL_Surface* surface, int newSize);
