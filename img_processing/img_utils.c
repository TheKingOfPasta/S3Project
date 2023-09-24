#include "img_utils.h"

/**
 * Loads the image into a surface in the RGB888 format
 * and return it
 * <path>: The image path to load
  */
SDL_Surface* IMGU_Load(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);

    if (tmp == NULL)
        errx(EXIT_FAILURE, "%s",  SDL_GetError());

    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888,
            0);

    SDL_FreeSurface(tmp);

    return surface;
}


