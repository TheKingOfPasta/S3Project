#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <math.h>

void print_matrix(double** arr, size_t size)
{
	for (size_t j = 0; j < size; j++)
	{
		for (size_t i = 0; i < size; i++)
		{
			printf("%02f", arr[i][j]);
		}
		printf("\n");
	}
}

SDL_Surface* IMGA_GaussianBlur(SDL_Surface* surface, int size, double sigma)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);

	double gaussianWeights[size][size];

	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < size; i++)
		{
			double i2 = i - size / 2;
			double j2 = j - size / 2;
			gaussianWeights[i][j] = 1000 * exp(-(i2*i2 + j2*j2) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
		}
	}

	/*int gaussianWeights[5][5] =
    {
        {1,4,7,4,1},
        {4,16,26,16,4},
        {7,26,41,26,7},
        {4,16,26,16,4},
        {1,4,7,4,1}
    };*/

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
