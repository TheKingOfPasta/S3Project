// IMGC FILE
// === IMAGE COLOR LIBRARY

#include "img_color.h"

/**
  * Convert the given surface into grayscale
  */
SDL_Surface* IMGC_surface_to_grayscale(SDL_Surface* surface)
{

  SDL_Surface* newS =
      SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

  Uint32* pixels = surface->pixels;
  Uint32* newPixels = (newS->pixels);

  SDL_PixelFormat* format = surface->format;

  SDL_LockSurface(newS);
  Uint8 r, g, b;

  for (int i = 0; i < surface->w; ++i)
  for (int j = 0; j < surface->h; ++j)
  {
    int offset = j * surface->pitch + i * format->BytesPerPixel;
    SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format, &r, &g, &b);
    Uint8 avr = 0.3 * r + 0.59 * g + 0.11 * b;
    newPixels[i+surface->w*j] = SDL_MapRGB(newS->format, avr, avr, avr);
  }

  SDL_UnlockSurface(newS);
  SDL_FreeSurface(surface);
  return newS;
}