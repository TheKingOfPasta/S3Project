#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "adaptive_thresholding.h"

SDL_Surface* IMGA_ApplyThreshold(SDL_Surface* surface, int threshold)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    int splitSize = 21;

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);

    Uint8 r,g,b;

    for (int i = 0; i < surface->w; i += splitSize)
    for (int j = 0; j < surface->h; j += splitSize)
    {
        int count = splitSize * splitSize;
        int sum = 0;
        for (int k = 0; k < splitSize; k++)
        for (int l = 0; l < splitSize; l++)
        {
            if (i + k >= surface->w || j + l >= surface->h)
                continue;
            
            SDL_GetRGB(
                    pixels[(i + k) * surface->h + j + l],
                    surface->format,
                    &r,
                    &g,
                    &b);
            
            sum += r;
        }

        Uint32 average = sum/count - threshold;

        for (int k = 0; k < splitSize; k++)
        for (int l = 0; l < splitSize; l++)
        {
            if (i + k >= surface->w || j + l >= surface->h)
                continue;
            
            SDL_GetRGB(
                    pixels[(i + k) * surface->h + j + l],
                    surface->format,
                    &r,
                    &g,
                    &b);

            //printf("%i <> %i\n", r, average);
            newPixels[(i + k) * surface->h + j + l] =
					(r >= average) ?
                SDL_MapRGB(newS->format, 0, 0, 0) :
                SDL_MapRGB(newS->format, 255, 255, 255);
        }
    }

    SDL_FreeSurface(surface);

    return newS;
}
