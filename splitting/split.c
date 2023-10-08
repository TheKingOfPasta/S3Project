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
        errx(EXIT_FAILURE, "noo");
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "yes");
    SDL_FreeSurface(s);
    return surface;
}

Uint32** Splitting(SDL_Surface* surface)
{
    Uint32* image = surface->pixels;
    Uint32** ImSplit = malloc(81*sizeof(Uint32*));
    SDL_LockSurface(surface);
    int width = surface->w;
    //width = width - width%9;
    int height = surface->h;
    //height = height - height%9;
    int len = (width/9) * (height/9);
    for(int j = 0; j<9; j++)
    {
        for(int i = 0; i<9;i++)
        {
            Uint32* Im = malloc(len*sizeof(Uint32));
            int x = 0;
            for(int h = j*(height/9)*width; h<((j+1)*(height/9)*width);h+=width)
            {
		    h++;
                for(int w = i*(width/9);w<(i+1)*(width/9);w++,x++)
                {
		    Im[x] = image[w+h];
		    printf("p = %li\n",image[w+h]);
                }
            }
	    ImSplit[j*9+i] = Im;
        }
    }
    SDL_UnlockSurface(surface)	;
    return ImSplit;
}

int main(int argc,char **argv)
{
        if (argc != 2)
                errx(1,"only 1 parameter");
        SDL_Surface* surface = load_image(argv[1]);
        Uint32** im = Splitting(surface);
	int width = surface->w/9;
	int pitch = surface->pitch;
	Files(im[0],"test0.png",width,width,pitch);
	Files(im[1],"test1.png",width,width,pitch);
	Files(im[2],"test2.png",width,width,pitch);
	Files(im[3],"test3.png",width,width,pitch);
	Files(im[4],"test4.png",width,width,pitch);
	Files(im[5],"test5.png",width,width,pitch);
	Files(im[6],"test6.png",width,width,pitch);
	Files(im[7],"test7.png",width,width,pitch);
	Files(im[8],"test8.png",width,width,pitch);
	Files(im[9],"test9.png",width,width,pitch);
	Files(surface->pixels,"pixels.png",surface->w,surface->w,pitch);
	SDL_FreeSurface(surface);
	SDL_Quit();
	free(im);
        return 0;
}



void Files(Uint32* image, const char* output, int width, int height, int pitch)
{
	SDL_Surface* s = SDL_CreateRGBSurfaceFrom(image, width, height, 32, pitch, 0, 0, 0, 0);
	SDL_Surface* surface = SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_RGB888,0);
	IMG_SavePNG(surface,output);
	SDL_FreeSurface(s);
	SDL_FreeSurface(surface);
}


