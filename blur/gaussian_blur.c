#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>

SDL_Surface* BLUR_GaussianBlur(SDL_Surface* surface)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);

    for (int i = 0; i < surface->w; i++)
    for (int j = 0; j < surface->h; j++)
    {
        Uint8 r;
        Uint8 g;
        Uint8 b;

        int total = 0;
        int totalWeight = 0;

        for (int k = -1; k <= 1; k++)
        for (int l = -1; l <= 1; l++)
        {
            if (i + k >= 0 &&
                i + k < surface->w &&
                j + l >= 0 &&
                j + l < surface->h)
            {
                SDL_GetRGB(
                    pixels[i * surface->h + k + j + l],
                    surface->format,
                    &r,
                    &g,
                    &b);
				//r = pixels[(i+k) * surface->h + j + l] & 0b11111111000000000000000000000000;//always 0 lol
                int weight = 1<<(2 - abs(k) - abs(l));
                
                totalWeight += weight;
                total += weight * r;//Since r = b = g (grayscale)
                //printf("%i * %i\n", weight, r);
            }
        }

        newPixels[i * newS->h + j] =
            SDL_MapRGB(newS->format,
                        total / totalWeight,
                        total / totalWeight,
                        total / totalWeight);
    }

    SDL_FreeSurface(surface);

	return newS;
}

int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage : path_in path_out");

    IMG_SavePNG(BLUR_GaussianBlur(IMG_Load(argv[1])), argv[2]);

	return EXIT_SUCCESS;
}
