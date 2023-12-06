#include "hough.h"

#define LINE_THRESHOLD 40 // percent

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
	for (int t = 0; t < theta_num; t++)
	for (int r = 0; r < rho_num; r++)
	{
		if (accumulator[r][t] > maxVal)
			maxVal = accumulator[r][t];
	}

	Visualize_Acc(accumulator, diag_len * 2, maxVal);

    int line_threshold = maxVal * (LINE_THRESHOLD/100.0);

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

//Removes all lines which are too close to each other
void AveragesCloseLine(List* lLine, int diag_len)
{
	Node* curr;
    if (!(curr = lLine->head)) return;


	int  i =0;
    while (curr)
    {
		Line* currLine = curr->data;
        Node* curr2 = curr;
		double sumrho = currLine->rho;
		double sumtheta = currLine->theta;
		int nb =1;

		//printf("		%3i : theta %2.3f (deg %i) rho %5f\n",i,currLine->theta,(int)((currLine->theta)*180/M_PI),currLine->rho);
		int j =i+1;
		while (curr2->next)
		{
			Line* currLine2 = curr2->next->data;
			if (CloseAngle(currLine->theta, currLine2->theta,ToRad(30)) &&
				fabs(currLine2->rho - currLine->rho)/diag_len < 0.02)
				//   ^ I removed a fabs here if something is broken
			{
				//printf("			%3i : theta %2.3f (deg %i) rho %5f\n",j,currLine2->theta,(int)((currLine2->theta)*180/M_PI),currLine->rho);
				j++;
				//TODO average
				//and thetas can be equivalent
				nb++;
				sumrho+= currLine2->rho;
				if( fabs(currLine->theta-currLine2->theta)<M_PI_4)
					sumtheta+= currLine2->theta;
				else if(fabs(currLine->theta-currLine2->theta+M_PI)<M_PI_4)
					sumtheta+= currLine2->theta+M_PI;
				else sumtheta+= currLine2->theta-M_PI;
				RemoveNextNode(lLine,curr2);
			}
			else
				curr2 = curr2->next;
		}
		if(nb!=1){
			currLine->rho = sumrho/nb;
			currLine->theta = sumtheta/nb;
			//printf("		newvalu	%3i : theta %2.3f (deg %i) rho %5f\n",i,currLine->theta,(int)((currLine->theta)*180/M_PI),currLine->rho);
		}
		i++;
        curr = curr->next;
    }
}

//list has at 4 elements
void ExcludeBorderLine(List* list, int w, int h, double exthres)
{
    Node* curr = list->head;
    int count = 0;
	Node *arrToRemove[4];
	double widthbound = w*exthres;
	double heightbound = h*exthres;
    //printf("%i  %f %i %f\n",w,widthbound,h, heightbound);
	while (curr)
    {
        Line l = *(Line*)curr->data;
		if( CloseAngle(l.theta, ToRad(0), ToRad(2)) &&
				((fabs(l.rho)<widthbound) || (fabs(l.rho-w)<widthbound))){
			arrToRemove[count++] = curr;
				}
		else if(CloseAngle(l.theta, ToRad(90), ToRad(2)) &&
				((fabs(l.rho)<heightbound) || (fabs(l.rho-h)<heightbound)))
			arrToRemove[count++] = curr;
		curr =curr->next;
        if (count >= 4)
            return ;//Too many borders, don't exclude
    }
	/*
	for (int j =  0 ; j<count;j++){
		printf("line %i  theta %f rho %f\n",
			 j,((Line*)arrToRemove[j]->data)->theta,
			((Line*)arrToRemove[j]->data)->rho);
	}
	*/

	int i= 0;
	curr = list->head;
	while(i <= count && arrToRemove[i] == curr && curr->next)
	{
		Tail(list);
		curr = list->head;
		i++;
	}

	while( i <= count && curr->next){
		if (curr->next == arrToRemove[i]){
			RemoveNextNode(list,curr);
			i++;
		}
		else curr =curr->next;
	}
}

void RemovesStrayLine(List *l,int thresh, List* LVer, List* LHor){
	int histogram[180];
	for(int i =0; i<180;i++) histogram[i] =0;
	Node* curr = l->head;
	while(curr){
		histogram[(((int)(((Line*)(curr->data))->theta*180/M_PI))+90) %180]++;
		curr = curr->next;
	}

	int max = 0;
	int indexMax = 0;
	for (int i = 0; i < 180; i++)
	{
		if(histogram[i]>max){
			max = histogram[i];
			indexMax = i;
		}
	}

	int secondMax = (indexMax +90) %180;

	int checkAngles(int a, int b){
		return abs(a - b)<thresh || abs(a-b)> 180- thresh;
	};


	printf("indexMax %i   SecondMax %i\n", indexMax,secondMax);
	Node* tmp;
	curr = l->head;

	while(curr)
	{
		int currAngle = fabs((((Line*)(curr->data))->theta)*180/M_PI);
		if( checkAngles(currAngle,indexMax)){
			tmp = curr->next;
			curr = PrepopNode(l);
			InsertNode(LVer,curr);
			curr = tmp;
		}
		else if(checkAngles(currAngle,secondMax)){
			tmp = curr->next;
			curr = PrepopNode(l);
			InsertNode(LHor,curr);
			curr = tmp;
		}
		else{
			curr = curr->next;
		}
	}

}
