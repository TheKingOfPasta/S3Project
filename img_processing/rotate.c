#include "rotate.h"

/**
  * Rotate the texture with the given angle (from the center)
  *
  */

SDL_Surface* IMGA_Rotate(SDL_Surface* surface, double angle)
{
    double angleRad = angle * M_PI / 180;
    double cosine = cos(angleRad);
    double sine = sin(angleRad);

	SDL_Surface* newS = SDL_CreateRGBSurface(0,
							surface->h*fabs(sine)+surface->w*fabs(cosine),
							surface->w*fabs(sine)+surface->h*fabs(cosine), 32,0,0,0,0);

    SDL_LockSurface(surface);
    Uint32* pixels = surface->pixels;
    Uint32* newPixels = newS->pixels;

    for (int x = 0; x < newS->w; x++)
    {
		int x2 = x - newS->w / 2;
        for (int y = 0; y < newS->h; y++)
        {
			int y2 = y - newS->h / 2;

            int surfaceX = (int)(x2 * cosine - y2 * sine) + surface->w / 2;
            int surfaceY = (int)(y2 * cosine + x2 * sine) + surface->h / 2;

            if (surfaceX >= 0 && surfaceX < surface->w &&
                surfaceY >= 0 && surfaceY < surface->h)
            {
			    newPixels[y * newS->w + x] =
                    pixels[surfaceY * surface->w + surfaceX];
            }
			else //Out of bounds
            {
                newPixels[y * newS->w + x] =
                    SDL_MapRGB(newS->format,255,255,255);
            }
        }
	}

	SDL_UnlockSurface(surface);
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
double FindSkew(SDL_Surface* surface)
{
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
