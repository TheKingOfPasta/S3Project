#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>

#include "img_utils.h"

SDL_Surface* IMGA_Rotate_auto(char* path);//remove me
double FindSkew(SDL_Surface* surface);//remove me
double GetSkewFromFile(char* path);//remove me

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        printf("The skew of %s is %f\n", argv[2], GetSkewFromFile(argv[2]));

        return EXIT_SUCCESS;
    }
    if (argc != 4)
        errx(EXIT_FAILURE, "Usage: image_in path_out angle");
    char* path_in = argv[1];
    char* path_out = argv[2];
    char* endptr;
    double angle = strtod(argv[3], &endptr);

    return EXIT_SUCCESS;
}

/**
  * Rotate the texture with the given angle (from the center)
  *
  */
/*void IMGA_Rotate(SDL_Renderer* r, SDL_Texture* tex, const double angle)
{
    int error = SDL_RenderCopyEx(r, tex, NULL, NULL, angle, NULL,
            SDL_FLIP_NONE);
    if (error)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
}*/
SDL_Surface* IMGA_Rotate(SDL_Surface* surface, const double angle)
{
    //augus help
}


void IMGA_Rotate_from(char* path, const double angle)
{
    // TODO
}

Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
    return *((Uint32 *) ((Uint8 *) surface->pixels
                + y * surface->pitch
                + x * surface->format->BytesPerPixel));
}

//A high value for well rotated images and a low value for badly rotated ones
//If you see this then ping me for now idk what the "high" and "low" values will be but I plan on completing this function comment with those
double FindSkew(SDL_Surface* surface)
{
    //TODO : this will probably break if there are too many random black lines/points on the side
    SDL_LockSurface(surface);

    double coef = 0;

    //Start at 1 and end at -1
    // so you avoid having edge cases on the well... edges
    //This func works by checking for every black pixel if there are black
    // pixels left of right, and adds 2 to a global counter if so, if there
    // are black pixels on top or bottom of the pixel, it removes 1 to
    // the counter, this is used to prioritize horizontal lines
    for (int x = 1; x < surface->w - 1; x++)
        for (int y = 1; y < surface->h - 1; y++)
        {
            if (GetPixel(surface, x, y) == 0)
            {
                if (GetPixel(surface, x + 1, y) == 0)
                    coef += 1;
                if (GetPixel(surface, x - 1, y) == 0)
                    coef += 1;
                if (GetPixel(surface, x, y + 1) == 0)
                    coef -= 1;
                if (GetPixel(surface, x, y - 1) == 0)
                    coef -= 1;
            }
        }

    SDL_UnlockSurface(surface);

    //Some more testing might be required here
    return 1000 * coef / (surface->w * surface->h);
    // *1000 just to have a user readable coef (0 < coef < 50 approx)
}

//Testing only
double GetSkewFromFile(char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    return FindSkew(surface);
}

//Rotates the image at the path 'path'
//(searches the optimal rotation and calls IMGA_Rotate_from)
SDL_Surface* IMGA_Rotate_auto(char* path)
{
    SDL_Surface* surface = IMG_Load(path);
 
    double skewLeft = FindSkew(IMGA_Rotate(surface, 10));
    double currentSkew = FindSkew(surface);

    SDL_Surface* max = surface;
    double maxSkew = -1;

    double angleFactor = (skewLeft > currentSkew) ? 1 : -1;
    //If skewLeft > currentSkew, we need to rotate left, right otherwise

    for (double i = 1; i < 45; i++)
    {
        SDL_Surface* rotated = IMGA_Rotate(surface, angleFactor * i);
        double skew = FindSkew(rotated);
        if (skew > maxSkew)
        {
            maxSkew = skew;
            max = rotated;
        }
        else//If it's decreasing we went too far
            return max;
    }

    return max;
}
