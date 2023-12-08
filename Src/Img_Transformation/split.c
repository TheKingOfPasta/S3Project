#include "Img_Transformation/split.h"


void RecursiveExtraction(SDL_Surface* s,int x, int y,
					Uint8* l, Uint8* u, Uint8* r, Uint8* b){
	if (x<*l) *l = x;
	if (x>*r) *r = x;
	if (y<*u) *u = y;
	if (y>*b) *b = y;

    Uint32* pxls = s->pixels;
	Uint32 a = pxls[x+s->w*y];
	pxls[x+s->w*y] = 0;
	if (a!= pxls[x+s->w*y]){
		if(x!=0){
			if(y!=0) RecursiveExtraction(s,x-1,y-1,l,u,r,b);
			RecursiveExtraction(s,x-1,y,l,u,r,b);
			if(y!=s->h-1) RecursiveExtraction(s,x-1,y+1,l,u,r,b);
		}
		if(x!=s->w-1){
			if(y!=0) RecursiveExtraction(s,x+1,y-1,l,u,r,b);
			RecursiveExtraction(s,x+1,y,l,u,r,b);
			if(y!=s->h-1) RecursiveExtraction(s,x+1,y+1,l,u,r,b);
		}
		if(y!=0) RecursiveExtraction(s,x,y-1,l,u,r,b);
		if(y!=s->h-1) RecursiveExtraction(s,x,y+1,l,u,r,b);
	}
}

SDL_Surface* DigitExtraction(SDL_Surface* s){
	SDL_Surface* copy = SDL_CreateRGBSurface(0,s->w,s->h,32,0,0,0,0);
    Uint32* cppxls = copy->pixels;
    Uint32* pxls = s->pixels;
	SDL_PixelFormat* format = s->format;

    for (int i = 0; i < s->w; i++)
    for (int j = 0; j < s->h; j++){
        cppxls[i + j * s->w] = pxls[i + j * s->w];
    }

	int midw= s->w/2;
	int midh= s->h/2;

	Uint8 x =0,y=0,gray;
	Uint8 left,upper,right,bottom;
	int baseQuadSize = 1;

	while(0)
	{
		left = midw-baseQuadSize;
		upper = midh-baseQuadSize;
		right = midw+baseQuadSize;
		bottom = midh+baseQuadSize;

		for(int i = left; i<=right;i++)
		for(int j = upper; j<=bottom;j++)
		{
			int offset = i * s->pitch + left * format->BytesPerPixel;
			SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), format,
				&gray, &gray, &gray);
			if(gray){
				x=i;
				y=j;
			} goto foundOrigin;
		}
		baseQuadSize ++;
	}

	foundOrigin:
	RecursiveExtraction(copy,x,y,&left,&upper,&right,&bottom);
	SDL_FreeSurface(copy);

	SDL_Surface* res = SDL_CreateRGBSurface(0,right - left,bottom - upper,
											32,0,0,0,0);
    Uint32* respxls = s->pixels;

	for(int i = left; i<right;i++)
	for(int j = upper; j<bottom;j++)
	{
		respxls[i-left + ( j-upper)*res->w] = pxls[i+j*s->w];
	}

	SDL_FreeSurface(s);
	return res;
}


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

            newS = DigitExtraction(newS);
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
