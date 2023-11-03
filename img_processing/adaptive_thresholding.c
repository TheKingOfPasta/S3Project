#include "adaptive_thresholding.h"

SDL_Surface* IMGA_ApplyThreshold(SDL_Surface* surface, int threshold)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    int splitSize = 3;//TODO : resolution dependent size

    SDL_LockSurface(newS);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);
    Uint8 r,g,b;

    for (int i = 0; i < surface->w; i ++)
    for (int j = 0; j < surface->h; j ++)
    {
        int count = 0;
        Uint32 sum = 0;
        for (int k = -splitSize/2; k <= splitSize/2; k++)
        for (int l = -splitSize/2; l <= splitSize/2; l++)
        {
            int dx = i+k;
            int dy = j+l;
            if (dx >=0 && dx<surface->w && dy>=0 && dy<surface->h)
            {
                SDL_GetRGB(
                    pixels[dx + (dy) * surface->w],
                    surface->format,
                    &r,
                    &g,
                    &b);
                sum += r;
                count++;
            }
        }

        int average = sum/count;

        SDL_GetRGB(
            pixels[i + (j) * surface->w],
            surface->format,
            &r,
            &g,
            &b);
        newPixels[i+j*surface->w] =
            r< threshold +average-3 ?
            SDL_MapRGB(surface->format, 255, 255, 255) :
            0;
    }

    SDL_UnlockSurface(newS);
    SDL_FreeSurface(surface);
    return newS;
}
