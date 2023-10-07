#define _GNU_SOURCE
#include <stdio.h> 
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* s = IMG_Load(path);
    if (s == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(s);
    return surface;
}

Uint32** Splitting(SDL_Surface* surface,SDL_Surface*** s)
{
        Uint32* image = surface->pixels;
    Uint32** ImSplit = malloc(81*sizeof(Uint32*));
    int width = surface->w;
    int height = surface->h;
    int len = width/9 * height/9;
    for(int j = 0; j<9; j++)
    {
        for(int i = 0; i<9;i++)
        {
            Uint32* Im = malloc(len*sizeof(Uint32));
            int x = 0;
            for(int h = j*width; h<(j+1)*width;h++)
            {
                for(int w = i*width/9;w<(i+1)*width/9;w++,x++)
                {
                    Im[x] = image[w+h];
                }
            }
            s[i+j] = SDL_CreateRGBSurfaceFrom(Im, width/9, height/9, 32, surface->pitch/*, surface->format*/, 0, 0, 0, 0);
            ImSplit[j+i] = Im;
        }
    }
    return ImSplit;
}
int main(int argc,char **argv)
{
        if (argc != 2)
                errx(1,"only 1 parameter");
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
                errx(EXIT_FAILURE, "%s", SDL_GetError());
        SDL_Surface* surface = load_image(argv[1]);
        SDL_Surface** s = malloc(81*sizeof(SDL_Surface*));
        Uint32** im = Splitting(surface,&s);
        SDL_FreeSurface(surface);
        for(int i = 0; i<81; i++)
        {
                char** c = malloc(8);
                *c = "test";
                char* l = *c;
                int p = asprintf(c,"%s%i.png",*c,i);
                free(l);
                SDL_SaveBMP(s[i],*c);
                SDL_FreeSurface(s[i]);
        }
        SDL_Quit();
        return 0;
}
