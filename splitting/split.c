#include "split.h"

//Fills 'folder_out' with 81 new images
void Split(SDL_Surface* surface, char* folder_out)
{
    Uint32* pixels = surface->pixels;

    int w9 = surface->w / 9;
    int h9 = surface->h / 9;

    int fileIndex = 1;

    for (int j = 0; j < surface->h - h9; j += h9)
    {
        for (int i = 0; i < surface->w - w9; i += w9)
        {
            printf("j: %i, i: %i\n", j, i);
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
            printf("saving\n");
            int e = IMG_SavePNG(newS, str);
            printf("done saving\n");
            if (e == -1)
                err(EXIT_FAILURE, "IMG_SavePNG");
            fileIndex += 1;
            free(str);
        }
    }

    // free the input surface but later on we need it so commented out
    //SDL_FreeSurface(surface);

    printf("done\n");
}
