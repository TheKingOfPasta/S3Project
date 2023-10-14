#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

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
    double angleRad = angle * 3.141592 / 180;
    double cosine = cos(angleRad);
    double sine = sin(angleRad);
	

	SDL_Surface* newS = SDL_CreateRGBSurface(0,
							surface->h*sine+surface->w*cosine,
							surface->w*sine+surface->h*cosine, 32,0,0,0,0);

    SDL_LockSurface(surface);

    for (int x = 0; x < newS->w; x+=1)
    {
		int x2 = x - newS->w / 2;
        for (int y = 0; y < newS->h; y+=1)
        {
			int y2 = y - newS->h / 2;

            int newX = (int)(x2 * cosine - y2 * sine) + surface->w / 2;
            int newY = (int)(y2 * cosine + x2 * sine) + surface->h / 2;

			((Uint32*)newS->pixels)[y*newS->w+x] = 
			 	( newX >= 0 && newX<surface->w && newY >= 0 && newY < surface->h)?
				((Uint32*)surface->pixels)[newY*surface->w+newX]:
				SDL_MapRGB(newS->format,255,255,255);
        }
	}

    printf("returning\n");
    SDL_FreeSurface(surface);
	SDL_UnlockSurface(surface);

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