#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>

SDL_Surface* IMGA_GaussianBlur(SDL_Surface* surface)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);



    int gaussianWeights[5][5] =
    {
        {1,4,7,4,1},
        {4,16,26,16,4},
        {7,26,41,26,7},
        {4,16,26,16,4},
        {1,4,7,4,1}
    };

    for (int i = 0; i < surface->w; i++)
    for (int j = 0; j < surface->h; j++)
    {
        Uint8 r;
        Uint8 g;
        Uint8 b;

        size_t total = 0;
        size_t totalWeight = 0;
        
        for (int k = -2; k <= 2; k++)
        for (int l = -2; l <= 2; l++)
        {
            if (i + k >= 0 &&
                i + k < surface->w &&
                j + l >= 0 &&
                j + l < surface->h)
            {
                //printf("%i/%i %i/%i\n", i, surface->w, j, surface->h);
                SDL_GetRGB(
                    pixels[i * surface->h + k + j + l],
                    surface->format,
                    &r,
                    &g,
                    &b);
                //printf("%i/%i %i/%i\n\n", i, surface->w, j, surface->h);
				//r = pixels[(i+k) * surface->h + j + l] & 0b11111111000000000000000000000000;//always 0 lol
                
                //size_t weight = 1<<(10 - abs(k) - abs(l));
                size_t weight = gaussianWeights[k + 2][l + 2];
                
                totalWeight += weight;
                total += weight * r;//Since r = b = g (grayscale)

            }
        }
        
        //printf("%zu / %zu == %zu\n", total, totalWeight, total/totalWeight);

        newPixels[i * newS->h + j] =
            SDL_MapRGB(newS->format,
                        total / totalWeight,
                        total / totalWeight,
                        total / totalWeight);
        
    }

    SDL_FreeSurface(surface);

	return newS;
}
