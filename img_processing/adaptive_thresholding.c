#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

    //new new code , slower , but who cares ? (it actually works)
    
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


    //New code probably worse (didn't think that was possible but hey, just look at it)
    Uint32 intArr[surface->w][surface->h];

    for (int i = 0; i < surface->w; i++)
    {
        int sum = 0;
        for (int j = 0; j < surface->h; j++)
        {
            sum += pixels[i + j * surface->w];
            if (!i)
                intArr[i][j] = sum;
            else
                intArr[i][j] = intArr[i - 1][j] + sum;
        }
    }

    for (int i = 0; i < surface->w; i++)
    {
        for (int j = 0; j < surface->h; j++)
        {
            int x1 = i - splitSize/2;
            int x2 = i + splitSize/2;
            int y1 = j - splitSize/2;
            int y2 = j + splitSize/2;

            if (x1 <= 0 || x2 <= 0 || y1 <= 0 || y2 <= 0 || x1 >= surface->w || x2 >= surface->w || y1 >= surface->h || y2 >= surface->h)
                continue;

            int count = (x2 - x1) * (y2 - y1);
            int sum = intArr[x2][y2] - intArr[x2][y1 - 1] - intArr[x1 - 1][y2] + intArr[x1 - 1][y1 - 1];
            if ((int)(pixels[i + j * surface->w] * count) <= sum * (100 - threshold)/100)
                newPixels[i + j * newS->w] = 0;
            else
                newPixels[i + j * newS->w] = SDL_MapRGB(newS->format, 255, 255, 255);
        }
    }

    return newS;

    //Old code wierd as heck


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
                    pixels[i + k + (j + l) * surface->w],
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
                    pixels[i + k + (j + l) * surface->w],
                    surface->format,
                    &r,
                    &g,
                    &b);

            newPixels[i + k + (j + l) * surface->w] =
					(r >= average) ?
                SDL_MapRGB(newS->format, 0, 0, 0) :
                SDL_MapRGB(newS->format, 255, 255, 255);
        }
    }

    SDL_FreeSurface(surface);

    return newS;
}
