#include "Img_Transformation/padding.h"

SDL_Surface* Padding(SDL_Surface* s, double paddingPercentage)
{
    int newW = s->w*(1+ paddingPercentage/100);
    int newH = s->h*(1+ paddingPercentage/100);
    SDL_Surface* pad = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);
    SDL_LockSurface(s);
    Uint32* pixels = pad->pixels;
    Uint32* p = s->pixels;
    for (int x=newW*paddingPercentage/200, xOg=0; xOg<s->w; xOg++, x++)
        for (int y=newH*paddingPercentage/200, yOg=0; yOg<s->h; yOg++, y++) {
            pixels[x+y*newW] = p[xOg + yOg * s->w];
        }
    SDL_UnlockSurface(s);
    SDL_FreeSurface(s);
    return pad;
}