#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img_action.h"
#include "gaussian_blur.h"
#include "adaptive_thresholding.h"


//1 -> a==b
//0 -> a!=b
int CompareStrings(char* a, char* b)
{
    for (; *a; a++)
    {
        if (*b != *a)
            return 0;
        b += 1;
    }
    return 1;
}

void ErrorMessage()
{
    errx(EXIT_FAILURE, "Usage: -r/--rotate\n                   -b/--blur\n                   -t/--threshold\n");
}

int main(int argc, char** argv)
{
    if (argc == 1)
        ErrorMessage();
    
    char* path_in = argv[2];
    char* path_out = argv[3];

    if (CompareStrings(argv[1], "-b") || CompareStrings(argv[1], "--blur"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-b/--blur : gaussian blur (path_in path_out)\n");
        
        printf("Attempting to blur image from %s\n", path_in);
        IMG_SavePNG(IMGA_GaussianBlur(IMG_Load(path_in)), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-r") || CompareStrings(argv[1], "--rotate"))
    {
        if (argc != 5)
            errx(EXIT_FAILURE, "-r/--rotate : rotate at path (path_in path_out angle)\n");
        char* endptr;
          double angle = strtod(argv[4], &endptr);

        printf("Attempting to rotate image from %s\n", path_in);
        IMG_SavePNG(IMGA_Rotate_from(path_in, angle), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-t") || CompareStrings(argv[1], "--threshold"))
    {
        if (argc != 5 && argc != 6)
            errx(EXIT_FAILURE, "-t/--threshold : apply adaptive thresholding (path_in folder_path_out threshold m) (m is optional : apply thresholds from 0 to n)\n");

        char* endptr;
        double threshold = strtod(argv[4], &endptr);

		char* str;

        printf("Attempting to apply thresholding image from %s\n", path_in);
		if (argc == 5)
			IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), threshold), path_out);
		else
		for (int i = 0; i <= threshold; i++)
		{
			asprintf(&str, "%s/thresholded_%i.png", path_out, i);	
			IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), i), str);
		}
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else
        ErrorMessage();

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
SDL_Surface* IMGA_Rotate(SDL_Surface* surface, double angle)
{
    angle += 1;
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, sqrt(2) * surface->w, sqrt(2) * surface->h, 32,0,0,0,0);
    double angleRad = /*angle * */10*3.141592 / 180;//Having angle breaks it apparently???????????
    int size = newS->w * newS->h;
    SDL_LockSurface(surface);

    for (int x = -surface->w/2; x <= surface->w/2; x+=1)
    {
        for (int y = -surface->h/2; y <= surface->h/2; y+=1)
        {
            double cosine = cos(angleRad);
            double sine = sin(angleRad);
            int newX = (int)(x * cosine - y * sine) + newS->w/2;
            int newY = (int)(y * cosine + x * sine) + newS->h/2;

            
            if ((newX * newS->h + newY < size) &&
               (newX * newS->h + newY>=0) &&
               (x * surface->h + y>=0) &&
               (x * surface->h + y < surface->w * surface->h))
            {
                ((Uint32*)(newS->pixels))[newX * newS->h + newY] =
                    ((Uint32*)(surface->pixels))[x * surface->h + y];
            }

                //printf("%i, %i  =  %i, %i\n", newX, newY, x, y);
        }
    }

    printf("returning\n");
    SDL_FreeSurface(surface);

    return newS;
}


SDL_Surface* IMGA_Rotate_from(char* path, const double angle)
{
    SDL_Surface* s = IMG_Load(path);
    printf("Sucessfully loaded the image\n");
    return IMGA_Rotate(s, angle);
}

Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
    return *((Uint32 *) ((Uint8 *) surface->pixels
                + y * surface->pitch
                + x * surface->format->BytesPerPixel));
}

//A high value for well rotated images and a low value for badly rotated ones
//If you see this then ping me for now idk what the "high)and "low" values will be but I plan on completing this function comment with those
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
 
    double skewLeft = FindSkew(IMGA_Rotate(surface, 10.0));
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
