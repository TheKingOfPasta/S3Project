#include "split.h"

//Fills 'folder_out' with 81 new images
void Split(SDL_Surface* surface, char* folder_out)
{
    Uint32* pixels = surface->pixels;

    int w9 = surface->w / 9;
    int h9 = surface->h / 9;

    int fileIndex = 1;

    for (int j = 0; j < surface->h; j += h9)
    {
        for (int i = 0; i < surface->w; i += w9)
        {
            SDL_Surface* newS = SDL_CreateRGBSurface(0, w9, h9, 32, 0, 0, 0, 0);
            Uint32* newPixels = newS->pixels;

            for (int x = 0; x < w9; x++)
            for (int y = 0; y < h9; y++)
            {
                newPixels[y * w9 + x] = pixels[(j + y) * surface->w + (i + x)];
            }

            char* str;
            if (asprintf(&str, "%s/split_%02i.png", folder_out, fileIndex) == -1)
                errx(EXIT_FAILURE, "asprintf failed");
            IMG_SavePNG(newS, str);
            fileIndex += 1;
            free(str);
        }
    }

    SDL_FreeSurface(surface);
}