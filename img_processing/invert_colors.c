#include "invert_colors.h"

//Takes a black and white image
//Inverts it if it has too much white
SDL_Surface* CheckInvert(SDL_Surface* img)
{
    Uint32* pixels = (Uint32*)(img->pixels);
    int whiteCellCount = 0;
    int blackCellCount = 0;

    for (int i = 0; i < img->w; i++)
    for (int j = 0; j < img->h; j++)
    {
        if ((Uint8)(pixels[i + j * img->w]) == 0)
            blackCellCount += 1;
        else
            whiteCellCount += 1;
    }

    if (whiteCellCount > blackCellCount)//Inverse
    {
        SDL_LockSurface(img);

        for (int i = 0; i < img->w; i++)
        for (int j = 0; j < img->h; j++)
        {
            int red = (Uint8)(pixels[i + j * img->w]);
            pixels[i + j * img->w] = SDL_MapRGB(img->format,
                                                255 - red,
                                                255 - red,
                                                255 - red);
        }

        SDL_UnlockSurface(img);
    }

    return img;
}