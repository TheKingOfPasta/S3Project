#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>


typedef struct NodeLine{
	struct NodeLine * next;
	double rho;
	int theta;
} NodeLine;

typedef struct ListLine {
	struct NodeLine* head;
} ListLine;

void MyDrawLine(SDL_Surface* s, int x1, int y1, int x2, int y2)
{
	//printf("trying to draw : w=%i h=%i ||x0 : %i  y0 : %i  x1 : %i  y1 : %i\n", s->w,s->h,x1,y1,x2,y2);

	int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx + dy;
    int e2;

	Uint32* pixels = (Uint32*)(s->pixels);

    while (x1 != x2 || y1 != y2)
    {
        if (0 <= x1 && x1 < s->w && 0 <= y1 && y1 < s->h)
        {
            pixels[x1 + y1 * s->w] = SDL_MapRGB(s->format, 255, 255, 255);
        }

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Visualize_Acc(unsigned int acc[][180] , int rmax, int maxacc)
{
	SDL_Surface* newS = SDL_CreateRGBSurface(0, 180, 200, 32,0,0,0,0);
	SDL_LockSurface(newS);
	Uint32* pixels = (Uint32*)(newS->pixels);
	for (int i = 0; i < newS->w; i++)
    for (int j = 0; j < newS->h; j++)
	{
		Uint8 value = (Uint8) ((((float)acc[j * rmax / (200)][i])/maxacc)*255);
		pixels[i+j*newS->w] = SDL_MapRGB(newS->format, value * 3, value * 3, value * 3);
	}
	SDL_UnlockSurface(newS);
	IMG_SavePNG(newS,"accumulator.png");
	SDL_FreeSurface(newS);
}

void spread_arr(int size, double min, double max, double step, double* array)
{
    double current = min;
    for (int i = 0; i < size && current <= max; i++)
    {
        array[i] = current;
        current += step;
    }
}

SDL_Surface* HoughLine(SDL_Surface* img, ListLine* line_list)
{
	int width = img->w;
	int height = img->h;
	
	int diag_len = ceil(sqrt(width * width + height * height));

	// Cache some resuable values
	double cos_t[180];
	double sin_t[180];
	for (int i = -90; i < 90; i++)
	{
		cos_t[i + 90] = cos(i * M_PI / 180.0);
		sin_t[i + 90] = sin(i * M_PI / 180.0);
	}
		
	// Hough accumulator array of theta vs rho
	unsigned int accumulator[2 * diag_len][180];
	for (int i = 0; i < 2 * diag_len; i++)
	for (int j = 0; j < 180; j++)
		accumulator[i][j] = 0u;
	
	// Accumulator calculation
	for (int i = 0; i < width; i += 1)
	for (int j = 0; j < height; j += 1)
	{
		int pixel = (int)((Uint32*)(img->pixels))[i + j * width];
		if (pixel < 127)continue;

		for (int angle = 0; angle < 180; angle += 1)
		{
			int rho = round(i * cos_t[angle] + j * sin_t[angle]) + diag_len;
			accumulator[rho][angle] += 1;
		}
	}


	SDL_Surface* newS = SDL_CreateRGBSurface(0,width, height, 32,0,0,0,0);

	unsigned int maxVal = 0;

	for (int i = 0; i < diag_len * 2; i++)
	for (int j = 0; j < 180; j++)
	{
		if (accumulator[i][j] > maxVal)
			maxVal = accumulator[i][j];
	}

	
	SDL_LockSurface(newS);

	Visualize_Acc(accumulator, diag_len * 2, maxVal);

    unsigned int line_threshold = maxVal / 2;//line threshold %

    int maxTheta = 0, maxRho = 0;
    int step = diag_len*2 / 60;

	double rho_min = -diag_len, rho_max = diag_len, rho_num = (diag_len * 2),
           rho_step = (rho_max - rho_min) / rho_num;

	double rhos[(int)(rho_num + 1)];
	
    spread_arr(rho_num + 1, rho_min, rho_max, rho_step, rhos);

	for (int t = 0; t < 180; t += step)
    for (int r = 0; r <= diag_len*2; r += step)
	{
		unsigned int val = accumulator[r][t];
		
		maxRho = r;
		maxTheta = t;

		for (int i = 0; i < step; i++)
		for (int j = 0; j < step; j++)
		{
			int x = r + i;
			int y = t + j;

			if (x >= diag_len*2 || y >= 180)
				continue;

			if (accumulator[x][y] > val)
			{
				val = accumulator[x][y];
				maxRho = x;
				maxTheta = y;
			}
		}

		if (val < line_threshold || (maxTheta != 0 && maxTheta != 90))
			continue;

		//drawing the lines on a surface
		double rho = rhos[maxRho];
		//double rho = maxRho - diag_len/2;

		NodeLine *nd = malloc(sizeof(NodeLine));
		
		nd->next = line_list->head;
		nd->rho = rho;
		nd->theta = maxTheta;
		line_list->head = nd;

		double c = cos_t[maxTheta];
		double s = sin_t[maxTheta];

		int x0 = rho * c;
		int y0 = rho * s;
		
		int x1 = x0 - diag_len * s;
		int y1 = y0 + diag_len * c;

		int x2 = x0 + diag_len * s;
		int y2 = y0 - diag_len * c;

		MyDrawLine(newS, x1, y1, x2, y2);
	}

	return newS;
}

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* s = IMG_Load(path);
    if (s == NULL)
	errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface =
			SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL)
	errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(s);
    return surface;
}


void printList(ListLine* l)
{
	printf("l = \n");
	NodeLine* temp = l->head;
	if(!temp) printf("empty\n");
	int i =1;
	while(temp){
		printf("%3i : theta %3i rho %f\n",i,temp->theta,temp->rho);
		i++;
		temp = temp->next;
	}
	//printf("%3i : theta %3i rho %f\n",i,temp->theta,temp->rho);
}

double DoubleAbs(double x)
{
	return x < 0 ? -x : x;
}

//Removes all lines which are too close to each other
void Prune(ListLine* l)
{
    if (!(l->head->next))
        return;


    NodeLine* curr = l->head;
	if (curr->next &&
		((curr->rho == curr->next->rho ||
			(curr->rho &&
			DoubleAbs(curr->next->rho / curr->rho - 1) < 0.1))
		&&
		(curr->theta == curr->next->theta ||
			(curr->theta &&
			DoubleAbs(
				(float)(curr->next->theta) /
				(float)(curr->theta) - 1)
				< 0.1))))
		{
			NodeLine* next = curr->next;
			curr->next = curr->next->next;
			free(next);
		}

    while (curr->next)
    {
        NodeLine* curr2 = curr->next->next;
		while (curr2)
		{
			if ((curr2->rho == curr->next->rho ||
					(curr2->rho &&
					DoubleAbs(curr->next->rho / curr2->rho - 1) < 0.1))
				&&
                (curr2->theta == curr->next->theta ||
					(curr2->theta &&
					DoubleAbs(
						(float)(curr->next->theta) /
						(float)(curr2->theta) - 1)
						< 0.1)))
			{
				
				curr->next->next = curr2->next;
				free(curr2);
				break;
			}
			curr2 = curr2->next;
		}

        curr = curr->next;
    }
}

SDL_Surface* ListToSurface(ListLine* list, int width, int height)
{
	SDL_Surface* newS = SDL_CreateRGBSurface(0, width, height, 32,0,0,0,0);
	SDL_LockSurface(newS);
	NodeLine* curr = list->head;

	int diag_len = sqrt(width * width + height * height);

	double cos_t[180];
	double sin_t[180];
	for (int i = -90; i < 90; i++)
	{
		cos_t[i + 90] = cos(i * M_PI / 180.0);
		sin_t[i + 90] = sin(i * M_PI / 180.0);
	}

	while (curr)
	{
		double c = cos_t[curr->theta];
		double s = sin_t[curr->theta];

		int x0 = curr->rho * c;
		int y0 = curr->rho * s;
		
		int x1 = x0 - diag_len * s;
		int y1 = y0 + diag_len * c;

		int x2 = x0 + diag_len * s;
		int y2 = y0 - diag_len * c;

		MyDrawLine(newS, x1, y1, x2, y2);

		curr = curr->next;
	}

	SDL_UnlockSurface(newS);
	return newS;
}

int main(int argc, char** argv){
	if (argc != 3)
		errx(1,"first param : path_in\nsecond param : path_out\n");
	SDL_Surface* input = load_image(argv[1]);
	ListLine list = { head : NULL };

	SDL_Surface* output = HoughLine(input, &list);
	Prune(&list);
	IMG_SavePNG(ListToSurface(&list, output->w, output->h), argv[2]);
	SDL_FreeSurface(input);
	SDL_FreeSurface(output);
	
	printf("\n\nAfter prune: ");
	printList(&list);
	
	return 0;
}