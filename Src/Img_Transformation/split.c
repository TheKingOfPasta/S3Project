#include "Img_Transformation/split.h"

void RecursiveRemoval(SDL_Surface* s, int x, int y){
	Uint32* pxls = s->pixels;
	Uint8 gray;
    int offset = y * s->pitch + x * s->format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), s->format,
			&gray, &gray, &gray);

	pxls[x+s->w*y] = SDL_MapRGB(s->format, 0,0,0);

	if (gray== 255){
		if(x!=0) RecursiveRemoval(s,x-1,y);
		if(x!=s->w-1) RecursiveRemoval(s,x+1,y);

		if(y!=0) RecursiveRemoval(s,x,y-1);
		if(y!=s->h-1) RecursiveRemoval(s,x,y+1);
	}
}

void RemoveBorder(SDL_Surface* s){
	double exclude = 20;
	for(int i=0; i<s->w;i++){
		if(i > s->w*(exclude/100) && i< s->w*(1-exclude/100)) continue;
		RecursiveRemoval(s, i, 0);
		RecursiveRemoval(s, i, s->h-1);
	}
	for(int j=0; j<s->h;j++){
		if(j > s->h*(exclude/100) && j< s->h*(1-exclude/100)) continue;
		RecursiveRemoval(s, 0, j);
		RecursiveRemoval(s, s->w-1, j);
	}
}

void RecursiveExtraction(SDL_Surface* s,int x, int y,
					Uint8* l, Uint8* u, Uint8* r, Uint8* b, int rec){
	Uint32* pxls = s->pixels;
	Uint8 gray;
    int offset = y * s->pitch + x * s->format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pxls + offset)), s->format,
			&gray, &gray, &gray);

	pxls[x+s->w*y] = SDL_MapRGB(s->format, 0,0,0);

	if (x<*l) *l = x;
	else if (x>*r) *r = x;
	if (y<*u) *u = y;
	else if (y>*b) *b = y;

	if(gray ==255)
	{
		rec = s->w >80 ? 3 : 1;
	}

	if (rec>0){
		if(x!=0){
			if(y!=0) RecursiveExtraction(s,x-1,y-1,l,u,r,b,rec-1);
			RecursiveExtraction(s,x-1,y,l,u,r,b,rec-1);
			if(y!=s->h-1) RecursiveExtraction(s,x-1,y+1,l,u,r,b,rec-1);
		}
		if(x!=s->w-1){
			if(y!=0) RecursiveExtraction(s,x+1,y-1,l,u,r,b,rec-1);
			RecursiveExtraction(s,x+1,y,l,u,r,b,rec-1);
			if(y!=s->h-1) RecursiveExtraction(s,x+1,y+1,l,u,r,b,rec-1);
		}
		if(y!=0) RecursiveExtraction(s,x,y-1,l,u,r,b,rec-1);
		if(y!=s->h-1) RecursiveExtraction(s,x,y+1,l,u,r,b,rec-1);
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

	return SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);

	foundOrigin:
	//printf("x:%i y:%i\n",x,y);
	RecursiveExtraction(copy,x,y,&left,&upper,&right,&bottom,1);
	//printf("l:%i u:%i r:%i b:%i\n",left,upper,right,bottom);
	//IMG_SavePNG(copy,"copy.png");
	SDL_FreeSurface(copy);

	int newWidth = right-left+1;
	int newHeight = bottom-upper+1;

	int maxWH = newWidth > newHeight ? newWidth : newHeight;

	res = SDL_CreateRGBSurface(0,maxWH,maxWH,32,0,0,0,0);
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
            SDL_Surface* newS = SDL_CreateRGBSurface(0, w9, h9, 32, 0, 0, 0, 0);
            Uint32* newPixels = newS->pixels;

            for (int x = 0; x < w9; x++)
            for (int y = 0; y < h9; y++)
            {
                newPixels[y * w9 + x] = pixels[(j + y) * surface->w + (i + x)];
            }

	    	char* str;
            if (asprintf(&str, "%s/split_%02i.png", folder_out, fileIndex) == -1)
                errx(EXIT_FAILURE, "asprintf failed");

            int e = IMG_SavePNG(newS, str);

            if (e == -1)
                err(EXIT_FAILURE, "IMG_SavePNG");
            free(str);

			char* f;
			if (asprintf(&f, "%s/split_%02i_28x28.png", folder_out, fileIndex) == -1)
				errx(EXIT_FAILURE, "asprintf failed");


			RemoveBorder(newS);
            newS = DigitExtraction(newS);
			newS = Padding(newS, 20);
            SDL_Surface *s = downscale_resize(newS, 28, 28);
			IMG_SavePNG(s, f);

            fileIndex += 1;
			SDL_FreeSurface(s);
			free(f);
        }
    }

    // free the input surface but later on we need it so commented out
    //SDL_FreeSurface(surface);
}
