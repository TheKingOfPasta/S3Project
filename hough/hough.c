#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>

void MyDrawLine(SDL_Surface* s, int x, int y, int x2, int y2)
{
    double vx = x2 - x;
    double vy = y2 - y;
    double len = sqrt(vx * vx + vy * vy);
    if (!len)
        return;//drawing from same point to same point

    vx /= len;
    vy /= len;
    double posX = x;
    double posY = y;

    Uint32* pixels = s->pixels;

    while (len)
    {
        pixels[(int)(posX + posY * s->w)] = SDL_MapRGB(s->format, 255, 0, 0);
        posX += vx;
        posY += vy;

        len -= 1;
    }
}

void Visualize_Acc(int acc[][180] , int rmax){
	int maxacc = 0;
	for (int i = 0; i < rmax; i++)
    for (int j = 0; j < 180; j++)
	{
		if (acc[i][j]>maxacc) maxacc = acc[i][j];
 	}

	
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
}

int** Fill_Matrix_Line(SDL_Surface* input,int threshold,int* nbLines){
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
						acc[rho][theta] += 1;
				}
			}
		}
	}

	Visualize_Acc(acc , rmax);
	
	int** m = malloc(20*sizeof(int*));
	int t = 0;
	int max = 20;
	for(int i = 0; i<(int)rmax; i++)
		for(int j = 0; j<180;j++)
		{
			if (acc[i][j] > threshold)
			{
				if (t == max) {
					//int** p  = m;
					m = realloc(m,(max+10)*sizeof(int*));
					//free(p);
					max += 10;
				}
				m[t] = malloc(2*sizeof(int));
				m[t][0] = i;
				m[t][1] = j;
				t++;
			}	
		}
	//int** p = m;
	*nbLines = t;
	m = realloc(m,t*sizeof(int*));
	//free(p);
	return m;
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
		errx(1,"first param : path\nsecond param: threshold\n");
	SDL_Surface* input = load_image(argv[1]);
	int nbLines;
	int** acc = Fill_Matrix_Line(input,Str_to_Int(argv[2]),&nbLines);
	printf("NbLines: %i\n",nbLines);
	draw_lines(input,acc,nbLines);
	SDL_FreeSurface(input);
	return 0;
}
