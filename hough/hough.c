#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>


#define distance(xa,ya,xb,yb) (sqrt((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb)))


void MyDrawLine(SDL_Surface* s, int x1, int y1, int x2, int y2)
{
	//printf("trying to draw : w=%i h=%i ||x0 : %i  y0 : %i  x1 : %i  y1 : %i\n", s->w,s->h,x,y,x2,y2);

	int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = (dx > dy ? dx : -dy) >>1;
    int e2;

    while (x1 < s->w && x1>0 && y1 < s->h && y1>0)  {
        Uint32* pixel = (Uint32*)((Uint8*)s->pixels + y1 * s->pitch + x1 * s->format->BytesPerPixel);
        *pixel = SDL_MapRGB(s->format, 255,  255,  255);
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
	
	while (x2 < s->w && x2>0 && y2 < s->h && y2>0) {
        Uint32* pixel = (Uint32*)((Uint8*)s->pixels + y2 * s->pitch + x2 * s->format->BytesPerPixel);
        *pixel = SDL_MapRGB(s->format, 255,  255,  255);
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x2 -= sx;
        }
        if (e2 < dy) {
            err += dx;
            y2 -= sy;
        }
    }
}

void Visualize_Acc(int acc[][180] , int rmax, int maxacc){

	SDL_Surface* newS =SDL_CreateRGBSurface(0,rmax, 180, 32,0,0,0,0);
	SDL_LockSurface(newS);
	Uint32* pixels = (Uint32*)(newS->pixels);
	for (int i = 0; i < newS->w; i++)
    for (int j = 0; j < newS->h; j++)
	{
		Uint32 value = (Uint32) ((((float)acc[i][j])/maxacc)*255);
		pixels[i+j*newS->w] = SDL_MapRGB(newS->format, value ,  value,  value);
	}
	SDL_UnlockSurface(newS);
	IMG_SavePNG(newS,"e.png");
	SDL_FreeSurface(newS);
}

SDL_Surface* Fill_Matrix_Line(SDL_Surface* input){
	//Determine the range of rho and theta :
	int rmax = (int)sqrt((input->w*input->w) + (input->h*input->h));

	//create a 2D array with dimensions (num rhos, num thetas)
	int acc[rmax][180];
	
	//set values to 0
	for(int i = 0; i<rmax; i++)
	{
		for(int j = 0; j<180; j++) {
			acc[i][j] = 0;
		}
	}


	int maxacc = 0;
	//check each pixel on the edge picture
	for(int y = 0; y<input->w;y++)
	{
		for(int x = 0; x<input->h;x++)
		{
			Uint8 pixel = ((Uint8*)input->pixels)[y * input->pitch + x];
            if (pixel > 128) {
				//loop over all the possible values of theta
				for(int theta = 0; theta < 180; theta++) //(0<=theta<Pi)
				{
					double radians = theta* M_PI/180.0;
					//compute the corresponding rho
					int rho = (int)(x*sin(radians) + y*cos(radians));
					if (rho > 0 && rho <rmax)
					//locate the theta and rho index in the accumulator and increment
						if((++acc[rho][theta])>maxacc) maxacc = acc[rho][theta];
						
				}
			}
		}
	}

	for (int i = 0; i < rmax; i++)
    for (int j = 0; j < 180; j++)
	{
		if (acc[i][j]>maxacc) maxacc = acc[i][j];
 	}

	Visualize_Acc(acc , rmax,maxacc);

	SDL_Surface* newS =SDL_CreateRGBSurface(0,input->w, input->h, 32,0,0,0,0);
	SDL_LockSurface(newS);
	//Uint32* pixels = (Uint32*)(newS->pixels);
	int width = newS->w;
	int height = newS->h;
	int t = 0;
	for(int i = 0; i<width; i++)
		for(int j = 0; j<height;j++)
		{
			if (acc[i][j] > maxacc*0.8)
			{
				t++;
				double radians = t * M_PI/180.0;
				int x0 = (int)(acc[i][0] * cos(radians));
                int y0 = (int)(acc[i][0] * sin(radians));
                int x1 = x0 + 100 * (-sin(radians));
                int y1 = y0 + 100 * (cos(radians));
                if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                   	MyDrawLine(newS, x0 + width / 2, y0 + height / 2, x1 + width / 2, y1 + height / 2);
				}

			}
		}

	return newS;
}

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

int Str_to_Int(char* str) {
	int total = 0;
	for(int i = 0; str[i] != '\0';i++)
	{
		total *= 10;
		total += str[i] - '0';
	}
	return total;
}

void draw_lines(SDL_Surface* sur, int** acc, int nbLines) {
	for(int i = 0; i<nbLines; i++) {
		for(int t = 0; t<180; t++)
		{
			double radians = t * M_PI/180.0;
			int x0 = (int)(acc[i][0] * cos(radians));
                	int y0 = (int)(acc[i][0] * sin(radians));
                	int x1 = x0 + 1000 * (-sin(radians));
                	int y1 = y0 + 1000 * (cos(radians));
                	if (x1 >= 0 && x1 < sur->w && y1 >= 0 && y1 < sur->h) {
                    		MyDrawLine(sur, x0 + sur->w / 2, y0 + sur->h / 2, x1 + sur->w / 2, y1 + sur->h / 2);
			}
		}
	}
	IMG_SavePNG(sur, "test.png");
}

int main(int argc, char** argv){
	if (argc != 3)
		errx(1,"first param : path_in\nsecond param : path_out\n");
	SDL_Surface* input = load_image(argv[1]);
	SDL_Surface* output  =Fill_Matrix_Line(input);
	IMG_SavePNG( output,argv[2]);
	SDL_FreeSurface(input);
	SDL_FreeSurface(output);
	return 0;
}
