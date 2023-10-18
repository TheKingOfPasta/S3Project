#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>

int** Fill_Matrix_Line(SDL_Surface* input,int threshold,int* nbLines){
	//Determine the range of rho and theta :
	double rmax = sqrt((input->w*input->w) + (input->h*input->h));
	printf("%f\n",rmax);
	//create a 2D array with dimensions (num rhos, num thetas)
	int acc[(int)rmax][180];
	
	//set values to 0
	

	//edge detection which i'm guessing is already in the input parameter ?
	Uint32* im = input->pixels;

	//check each pixel on the edge picture
	for(int i = 0; i<input->w;i++)
	{
		for(int j = 0; j<input->h;j++)
		{
			//if it is on edge 
			if(im[i+j*input->w] > 125000) //random test cause white pixels aren't all 255*255*255 
			{
				//loop over all the possible values of theta
				for(int theta = 0; theta < 180; theta++) //(0<=theta<Pi)
				{
					//compute the corresponding rho
					int rho = (int)(i*sin(theta) + j*cos(theta));

					//locate the theta and rho index in the accumulator and increment
					acc[rho][theta] += 1;
				}
			}
		}
	}
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
	/*iterate over the acc's values, retrieve the rho and theta index, and get the value of rho and
	theta from the index pair, which may be transformed back to the form y = ax+b if above a certain 
	threshold. i'm guessing the threshold is both for error and to have multiple lines cause if we only
       	took the biggest value it would only get one ?*/
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
		float a = cos(acc[i][1]);
		float b = sin(acc[i][1]);
		int rho = acc[i][0];
		float x0 = a*rho;
		float y0 = b*rho;
		int x1 = (int)(x0+1000*(-b));
		int y1 = (int)(y0+1000*(a));
		int x2 = (int)(x0 - 1000*(-b));
		int y2 = (int)(y0 - 1000*(a));
		printf("\nx1 = %i\ny1 = %i\nx2 = %i\ny2 = %i\n",x1,y1,x2,y2);
		printf("theta : %i\nrho : %i\n",acc[i][1],acc[i][0]);
		MyDrawLine(sur,x1,y1,x2,y2);
		//if (SDL_RenderDrawLine(sur,x1,y1,x2,y2))
		//	printf("no lol\n");
		//if(SDL_RenderDrawLine(sur,0,10,100,300))
		//	printf("cringe\n");
	}
	IMG_SavePNG(sur, "test.png");
}

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
        pixels[(int)(posX + posY * s->w)] = SDL_MapRGB(s->format, 255, 255, 255);
        posX += vx;
        posY += vy;

        len -= 1;
    }
}

int main(int argc, char** argv){
	if (argc != 3)
		errx(1,"first param : path\nsecond param: threshold\n");
	SDL_Surface* sur = load_image(argv[1]);
	int nbLines;
	int** v = Fill_Matrix_Line(sur,Str_to_Int(argv[2]),&nbLines);
	printf("NbLines: %i\n",nbLines);
	draw_lines(sur,v,nbLines);
	return 0;
}
