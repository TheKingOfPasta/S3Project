#include <err.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

SDL_Surface* Resize(SDL_Surface* surface, int newSize);

SDL_Surface* Resample(SDL_Surface* surface, int newSize);
