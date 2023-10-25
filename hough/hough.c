#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>


#define distance(xa,ya,xb,yb) (sqrt((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb)))

void MyDrawLine(SDL_Surface* s, int x1, int y1, int x2, int y2)
{
	printf("trying to draw : w=%i h=%i ||x0 : %i  y0 : %i  x1 : %i  y1 : %i\n", s->w,s->h,x1,y1,x2,y2);

	int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx + dy;
    int e2;

    while (x1 != x2 || y1 != y2)
    {
        if (0 <= x1 && x1 < s->w && 0 <= y1 && y1 < s->h)
        {
            ((Uint32*)(s->pixels))[x1 + y1 * s->w] = SDL_MapRGB(s->format, 255, 255, 255);
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
	SDL_Surface* newS = SDL_CreateRGBSurface(0, 180 * 2, 180, 32,0,0,0,0);
	SDL_LockSurface(newS);
	Uint32* pixels = (Uint32*)(newS->pixels);
	for (int i = 0; i < newS->w; i++)
    for (int j = 0; j < newS->h; j++)
	{
		Uint8 value = (Uint8) ((((float)acc[i * rmax / (180 * 2)][j])/maxacc)*255);
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

SDL_Surface* HoughLine(SDL_Surface* img)
{
	int width = img->w;
	int height = img->h;
	
	int diag_len = ceil(sqrt(width * width + height * height));

	// Cache some resuable values
	double cos_t[180];
	double sin_t[180];
	for (int i = 0; i < 180; i++)
	{
		cos_t[i] = cos(i * M_PI / 180.0);
		sin_t[i] = sin(i * M_PI / 180.0);
	}
		
	// Hough accumulator array of theta vs rho
	unsigned int accumulator[2 * diag_len][180];
	for (int i = 0; i < 2 * diag_len; i++)
	for (int j = 0; j < 180; j++)
		accumulator[i][j] = 0;
	

	// Accumulator calculation
	for (int i = 0; i < width; i += 1)
	{
		for (int j = 0; j < height; j += 1)
		{
			int pixel = (int)((Uint32*)(img->pixels))[i + j * width];
			if (pixel < 127)
				continue;
			for (int angle = 0; angle < 180; angle += 1)
			{
				int rho = round(i * cos_t[angle] + j * sin_t[angle]) + diag_len;

				accumulator[rho][angle] += 1;
			}
		}
	}

	SDL_Surface* newS = SDL_CreateRGBSurface(0,img->w, img->h, 32,0,0,0,0);

	unsigned int maxVal = 0;

	for (int i = 0; i < diag_len * 2; i++)
	{
		for (int j = 0; j < 180; j++)
		{
			if (accumulator[i][j] > maxVal)
				maxVal = accumulator[i][j];
		}
	}
	

	SDL_LockSurface(newS);

	double diag = sqrt(width * width + height * height);

	Visualize_Acc(accumulator, diag_len * 2, maxVal);
    printf("   👆 Maximum: %i\n", maxVal);

    // 2. Computing threshold
    unsigned int line_threshold = maxVal / 2;//line threshold %

    printf("   👈 Threshold: %u\n", line_threshold);

    // 3. Fiding coordinates of the edges in the accumulator using the
    // threshold

    int prev_t = 0, prev_r = 0;
    int step = diag_len*2 / 60;

	double rho_min = -diag, rho_max = diag, rho_num = (diag * 2),
           rho_step = (rho_max - rho_min) / rho_num;

	double rhos[(int)(rho_num + 2)];
	
    spread_arr(rho_num + 1, rho_min, rho_max, rho_step, rhos);

    // Creating the theta array
    double theta_min = -90, theta_max = 90, theta_num = rho_num,
           theta_step = (theta_max - theta_min) / theta_num;
    double thetas[(int)(theta_num + 2)];
    spread_arr(theta_num + 1, theta_min, theta_max, theta_step, thetas);

    for (int r = 0; r <= diag_len*2; r += step)
	{
		for (int t = 0; t <= 180; t += step)
		{
			unsigned int val = accumulator[r][t];
			prev_r = r;
			prev_t = t;

			// Looking for the maximum in a 10*10 window
			for (int i = 0; i < step; i++)
			{
				for (int j = 0; j < step; j++)
				{
					int x = r + i;
					int y = t + j;

					if (x > diag_len*2 || y > 180)
						continue;

					if (accumulator[x][y] > val)
					{
						val = accumulator[x][y];
						prev_r = x;
						prev_t = y;
					}
				}
			}

			if (val < line_threshold)
				continue;

			double rho = rhos[prev_r];
			double theta = thetas[prev_t];

			double ax = cos(theta);
			double ay = sin(theta);

			int x0 = rho * ax;
			int y0 = rho * ay;

			int x1 = x0 + diag * (-ay);
			int y1 = y0 + diag * (ax);

			int x2 = x0 - diag * (-ay);
			int y2 = y0 - diag * (ax);

			// 4. Draw edges on output
			MyDrawLine(newS, x1, y1, x2, y2);
		}
	}




	/*for(int i = 0; i<diag_len*2; i++)
	for(int j = 0; j<180;j++)
	{
		if (accumulator[i][j] > maxVal*0.8)
		{
			int x0 = (int)(accumulator[i][j] * cos_t[j]);
			int y0 = (int)(accumulator[i][j] * sin_t[j]);

			int x1 = x0 + 100 * (-cos_t[j]);
			if (!(x1 >= 0 && x1 < width))
				x1 = x0 - 100 * (-cos_t[j]);
			
			int y1 = y0 + 100 * (sin_t[j]);
			if (!(y1 >= 0 && y1 < height))//Put into bounds again
				y1 = y0 - 100 * (sin_t[j]);
			printf("%i %i %i %i \n", x0 + width / 2, y0 + height / 2, x1 + width / 2, y1 + height / 2);
			MyDrawLine(newS, x0 + width / 2, y0 + height / 2, x1 + width / 2, y1 + height / 2);
		}
	}*/

	return newS;
}

SDL_Surface* Fill_Matrix_Line(SDL_Surface* input){
	//Determine the range of rho and theta :
	int rmax = (int)sqrt((input->w*input->w) + (input->h*input->h));

	//create a 2D array with dimensions (num rhos, num thetas)
	unsigned int acc[rmax][180];
	
	//set values to 0
	for(int i = 0; i<rmax; i++)
	{
		for(int j = 0; j<180; j++)
		{
			acc[i][j] = 0;
		}
	}


	unsigned int maxacc = 0;
	//check each pixel on the edge picture
	for(int y = 0; y<input->w;y++)
	{
		for(int x = 0; x<input->h;x++)
		{
			Uint8 pixel = ((Uint8*)input->pixels)[y * input->pitch + x];
            if (pixel > 128)
			{
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
		if (acc[i][j]>maxacc)
			maxacc = acc[i][j];
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
                if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
				{
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

int Str_to_Int(char* str)
{
	int total = 0;
	for(int i = 0; str[i] != '\0';i++)
	{
		total *= 10;
		total += str[i] - '0';
	}
	return total;
}

void draw_lines(SDL_Surface* sur, int** acc, int nbLines)
{
	for(int i = 0; i<nbLines; i++)
	{
		for(int t = 0; t<180; t++)
		{
			double radians = t * M_PI/180.0;
			int x0 = (int)(acc[i][0] * cos(radians));
                	int y0 = (int)(acc[i][0] * sin(radians));
                	int x1 = x0 + 1000 * (-sin(radians));
                	int y1 = y0 + 1000 * (cos(radians));
                	if (x1 >= 0 && x1 < sur->w && y1 >= 0 && y1 < sur->h)
					{
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
	SDL_Surface* output = HoughLine(input);
	IMG_SavePNG(output, argv[2]);
	SDL_FreeSurface(input);
	SDL_FreeSurface(output);
	return 0;
}





















//Nothing here 👀


/*
static void find_edges(int **accumulator, int diag, int diag_len*2, int 180,
    double *rhos, double *thetas, int_list *edges_x, int_list *edges_y,
    Image *out, int w, int h, bool verbose_mode)
{

    printf("   📍 Fiding edges...\n");

    // 1. Fiding the maximum
    double max = 0;
    for (int r = 0; r <= diag_len*2; r++)

{
        for (int t = 0; t <= 180; t++)
    
	{
            int val = accumulator[r][t];

            if (val == 0)
                continue;

            if (val > max)
                max = val;
        }
    }

    printf("   👆 Maximum: %f\n", max);

    // 2. Computing threshold
    int line_threshold = max * (LINES_THRESHOLD_PERCENTAGE / 100.0);

    printf("   👈 Threshold: %i\n", line_threshold);

    // 3. Fiding coordinates of the edges in the accumulator using the
    // threshold

    int prev_t = 0, prev_r = 0;
    int step = diag_len*2 / 60;

    int r_c = 255, g_c = 0, b_c = 255;
    int edges = 0;

    for (int r = 0; r <= diag_len*2; r += step)

{
        for (int t = 0; t <= 180; t += step)
    
	{
            int val = accumulator[r][t];
            prev_r = r;
            prev_t = t;

            // Looking for the maximum in a 10*10 window
            for (int i = 0; i < step; i++)
        
		{
                for (int j = 0; j < step; j++)
            
			{
                    int x = r + i;
                    int y = t + j;

                    if (x > diag_len*2 || y > 180)
                        continue;

                    if (accumulator[x][y] > val)
                
				{
                        val = accumulator[x][y];
                        prev_r = x;
                        prev_t = y;
                    }
                }
            }

            if (val < line_threshold)
                continue;

            double rho = rhos[prev_r];
            double theta = thetas[prev_t];

            double ax = cos(theta);
            double ay = sin(theta);

            int x0 = rho * ax;
            int y0 = rho * ay;

            int x1 = x0 + diag * (-ay);
            int y1 = y0 + diag * (ax);

            int x2 = x0 - diag * (-ay);
            int y2 = y0 - diag * (ax);

            // 4. Draw edges on output
            int *coordinates
                = draw_line(out, w, h, x1, y1, x2, y2, r_c, g_c, b_c);
            edges++;

            li_append(edges_x, coordinates[0]);
            li_append(edges_y, coordinates[1]);

            li_append(edges_x, coordinates[2]);
            li_append(edges_y, coordinates[3]);

            free(coordinates);

            fprintf(stderr, "\33[2K\r   📊 Edges: %i", edges);

            if (g_c == 255)
        
		{
                r_c++;
                b_c--;
            }
            else if (b_c == 255)
        
		{
                r_c--;
                g_c++;
            }
            else
        
		{
                b_c++;
                r_c--;
            }
        }
    }

    fprintf(stderr, "\n");
}
*/