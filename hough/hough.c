#include "hough.h"

void spread_arr(int size, double min, double max, double step, double* array)
{
    double current = min;
    for (int i = 0; i < size && current <= max; i++)
    {
        array[i] = current;
        current += step;
    }
}

ListLine* HoughLine(SDL_Surface* img)
{
	int width = img->w;
	int height = img->h;
	
	int diag_len = ceil(sqrt(width * width + height * height));

	// Cache some resuable values
	double cos_t[180];
	double sin_t[180];
	for (int i = -45; i < 135; i++)
	{
		cos_t[i + 45] = cos(ToRad(i));
		sin_t[i + 45] = sin(ToRad(i));
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

	// getting the max value
	unsigned int maxVal = 0;
	for (int i = 0; i < diag_len * 2; i++)
	for (int j = 0; j < 180; j++)
	{
		if (accumulator[i][j] > maxVal)
			maxVal = accumulator[i][j];
	}

	Visualize_Acc(accumulator, diag_len * 2, maxVal);

    unsigned int line_threshold = maxVal *2/ 3;//line threshold %

    int maxTheta, maxRho;
    int step = diag_len*2 / 60;

	double rho_min = -diag_len, rho_max = diag_len, rho_num = (diag_len * 2),
           rho_step = (rho_max - rho_min) / rho_num;

	double rhos[(int)(rho_num + 1)];
	
    spread_arr(rho_num + 1, rho_min, rho_max, rho_step, rhos);

	ListLine* list = malloc(sizeof(ListLine)); 
	list->head = NULL;
	list->size =0;
	
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
		if (val < line_threshold)
			continue;

		NodeLine *nd = malloc(sizeof(NodeLine));
		nd->rho = rhos[maxRho];
		nd->theta = maxTheta-45%180;
		Preppend(list, nd);
	}
	return list;
}

//Removes all lines which are too close to each other
void Prune(ListLine* l)
{
	NodeLine* curr;
    if (!(curr = l->head))
	{
        return;
	}

    while (curr->next)
    {
        NodeLine* curr2 = curr;
		while (curr2->next)
		{
			if (CloseAngle(curr->theta, curr2->next->theta,5) &&
				DoubleAbs( DoubleAbs(curr2->next->rho / curr->rho)-1) < 0.05)
			{
				NodeLine* freeing = curr2->next;
				//TODO average (beware, rhos can be < 0) and thetas can be equivalent
				curr->rho = (curr->rho+freeing->rho)/2;
				curr->theta = (curr->theta+freeing->theta)/2;
				curr2->next = curr2->next->next;
				l->size--;
				free(freeing);
			}
			else
				curr2 = curr2->next;
		}

        curr = curr->next;
    }
}