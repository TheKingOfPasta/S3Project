#include "Grid_Detection/hough.h"

#define LINE_THRESHOLD 40// percent

void spread_arr(int size, double min, double max, double step, double* array)
{
    double current = min;
    for (int i = 0; i < size && current <= max; i++)
    {
        array[i] = current;
        current += step;
    }
}

// returns a list of the line found in the image
List* HoughLine(SDL_Surface* img)
{
	int width = img->w;
	int height = img->h;

	int diag_len = ceil(sqrt(width * width + height * height));

 	// Creating the rho values array used for line retrieval
    double rho_min = -diag_len, rho_max = diag_len, rho_num = (diag_len * 2);
	double rhos[((int)rho_num) + 1];
    spread_arr(rho_num + 1, rho_min, rho_max,
			(rho_max - rho_min) / rho_num,rhos);

    // Creating the theta array
    double theta_min = -45, theta_max = 135, theta_num = (diag_len * 2);
    double thetas[((int)theta_num) + 1];
    spread_arr(theta_num + 1, theta_min, theta_max,
			(theta_max - theta_min) / theta_num,thetas);

	// Cache some resuable values
    double cos_t[((int)theta_num) + 1];
    double sin_t[((int)theta_num) + 1];
    for (int i = 0; i <= theta_num; i++)
    {
        thetas[i] = ToRad(thetas[i]);
        cos_t[i] = cos(thetas[i]);
        sin_t[i] = sin(thetas[i]);
    }

    // Creating accumulator
    int **accumulator = malloc(sizeof(int *) * (rho_num + 1) + 1);
    for (int r = 0; r <= rho_num; r++)
    {
        accumulator[r] = malloc(sizeof(int) * (theta_num + 1) + 1);
        for (int t = 0; t <= theta_num; t++)
            accumulator[r][t] = 0;
    }

    Uint8 gray;
    Uint32 *pixels = img->pixels;
    // Accumulator calculation
	for (int i = 0; i < width; i++)
	for (int j = 0; j < height; j++)
	{
	    SDL_GetRGB(pixels[i+j*width],img->format,&gray,&gray,&gray);

            int pixel = gray;
		if (pixel < 127)continue;

		for (int angle = 0; angle < theta_num; angle += 1)
		{
			int rho = round(i * cos_t[angle] + j * sin_t[angle]) + diag_len;
			accumulator[rho][angle] += 1;
		}
	}

	// getting the max value
	int maxVal = 0;
	int secondMax = 0;
	int thirdMax = 0;
	for (int t = 0; t < theta_num; t++)
	for (int r = 0; r < rho_num; r++)
	{
		if (accumulator[r][t] > maxVal)
		{
			thirdMax = secondMax;
			secondMax = maxVal;
			maxVal = accumulator[r][t];
		}
		else if (accumulator[r][t] > secondMax){
			thirdMax = secondMax;
			secondMax = accumulator[r][t];
		}
		else if (accumulator[r][t] > thirdMax){
			thirdMax = accumulator[r][t];
		}
	}

	Visualize_Acc(accumulator, diag_len * 2, maxVal);

	int a = thirdMax/maxVal;
	int b = a*a;
    int line_threshold = maxVal * (LINE_THRESHOLD/100.0) *b*b;

    int maxTheta, maxRho;
    int step = diag_len*2 / 60;


	List* list = InitList();

	for (int t = 0; t < theta_num; t += step)
    for (int r = 0; r < rho_num; r += step)
	{
		 int val = accumulator[r][t];

		maxRho = r;
		maxTheta = t;

		for (int i = 0; i < step; i++)
		for (int j = 0; j < step; j++)
		{
			int x = r + i;
			int y = t + j;

			if (x > rho_num || y > theta_num)
				continue;

			if (accumulator[x][y] > val)
			{
				val = accumulator[x][y];
				maxRho = x;
				maxTheta = y;
			}
		}
		if (val < line_threshold ||
			fabs (fabs( thetas[maxTheta]) - M_PI_4) < ToRad(5))
			continue;

		Line *line = malloc(sizeof(Line));

		line->rho = rhos[maxRho];
		line->theta =thetas[maxTheta];

		Preppend(list, line);
	}

	for (int r = 0; r <= rho_num; r++)
    	free(accumulator[r]);
	free(accumulator);

	return list;
}