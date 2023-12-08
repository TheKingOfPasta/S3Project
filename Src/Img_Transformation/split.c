#include "Img_Transformation/split.h"


void RecursiveExtraction(SDL_Surface* s,int x, int y,
					Uint8* l, Uint8* u, Uint8* r, Uint8* b){
	Uint32* pxls = s->pixels;
	Uint8 gray;
    int offset = y * s->pitch + x * s->format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), s->format,
			&gray, &gray, &gray);

	pxls[x+s->w*y] = SDL_MapRGB(s->format, 50,0,0);

	if (x<*l) *l = x;
	else if (x>*r) *r = x;
	if (y<*u) *u = y;
	else if (y>*b) *b = y;

	if (gray== 255){
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
	SDL_Surface* res;

    for (int i = 0; i < s->w; i++)
    for (int j = 0; j < s->h; j++){
        cppxls[i + j * s->w] = pxls[i + j * s->w];
    }

	int midw= s->w/2;
	int midh= s->h/2;

	Uint8 x =0,y=0,gray;
	Uint8 left,upper,right,bottom;
	int baseQuadSize = 1;


	// here is constant to change if not working
	int lengthmax =(s->h+s->w)/4*0.37;


	while(baseQuadSize<lengthmax)
	{
		left = midw-baseQuadSize;
		upper = midh-baseQuadSize;
		right = midw+baseQuadSize;
		bottom = midh+baseQuadSize;


		for(int i = left; i<=right;i++)
		for(int j = upper; j<=bottom;j++)
		{
			int offset = j * s->pitch + i * format->BytesPerPixel;
			SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), format,
					&gray, &gray, &gray);
			if(gray)
			{
				x=i;
				y=j;
				goto foundOrigin;
			}

		}
		baseQuadSize ++;
	}

	SDL_FreeSurface(s);
	res = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
	return res;

	foundOrigin:
	//printf("x:%i y:%i\n",x,y);
	RecursiveExtraction(copy,x,y,&left,&upper,&right,&bottom);
	//printf("l:%i u:%i r:%i b:%i\n",left,upper,right,bottom);
	//IMG_SavePNG(copy,"copy.png");
	SDL_FreeSurface(copy);

	int newWidth = right-left+1;
	int newHeight = bottom-upper+1;

	int maxWH = newWidth > newHeight ? newWidth : newHeight;

	res = SDL_CreateRGBSurface(0,maxWH,maxWH,
											32,0,0,0,0);
    Uint32* respxls = res->pixels;

	for(int i = left, ii=(maxWH-newWidth)/2; i<=right;i++,ii++)
	{
		respxls[ii] = SDL_MapRGB(res->format,255,255,255);
		for(int j = upper, jj=(maxWH-newHeight)/2; j<=bottom;j++,jj++)
		{
			int offset = j * s->pitch + i * format->BytesPerPixel;
			SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), format,
					&gray, &gray, &gray);
			respxls[ii + jj*res->w] = SDL_MapRGB(res->format,gray,gray,gray);
		}
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
