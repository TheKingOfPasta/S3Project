#include "Grid_Detection/line_filtering.h"

//Removes all lines which are too close to each other
void AveragesCloseLine(List* lLine, int diag_len,int angleThreshold, double rhoThreshold)
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
			if (CloseAngle(currLine->theta, currLine2->theta,ToRad(angleThreshold)) &&
					fabs(currLine2->rho - currLine->rho)/diag_len < rhoThreshold)
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


	//printList(l,1);
	//printf("indexMax %i   SecondMax %i\n", indexMax,secondMax);
	curr = l->head;

	while(curr)
	{
		int currAngle = fabs((((Line*)(curr->data))->theta)*180/M_PI);
		if( abs(currAngle- indexMax)<thresh ||
				abs(currAngle-indexMax)> 180- thresh){
			curr = PrepopNode(l);
			InsertNode(LVer,curr);
		}
		else if(abs(currAngle- secondMax)<thresh ||
				abs(currAngle-secondMax)> 180- thresh){
			curr = PrepopNode(l);
			InsertNode(LHor,curr);
		}
		else{
			curr = PrepopNode(l);
			free(curr->data);
    		free(curr);
		}
		curr = l->head;
	}

}

SDL_Surface* CopySurface(SDL_Surface* s)
{
    SDL_Surface* res = SDL_CreateRGBSurface(0,s->w,s->h,32,0,0,0,0);
    Uint32* newpixls = res->pixels;
    Uint32* pixls = s->pixels;

    for (int i = 0; i < s->w; i++)
    for (int j = 0; j < s->h; j++)
        newpixls[i + j * s->w] = pixls[i + j * s->w];

    return res;
}

int CheckLine(Line* l, SDL_Surface* s, int segmentThreshold){
	int diag_len = sqrt(s->w * s->w + s->h * s->h);

	double co = (cos((l->theta)));
	double si = (sin((l->theta)));

	int x0 = l->rho * co;
	int y0 = l->rho * si;

	int x1 = x0 - diag_len * si;
	int y1 = y0 + diag_len * co;

	int x2 = x0 + diag_len * si;
	int y2 = y0 - diag_len * co;

	int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx + dy;
    int e2;

	Uint32* pixels = (Uint32*)(s->pixels);
	SDL_PixelFormat* format = s->format;

	Uint8 gray;
	int whitePreviously = 0;
	int whiteSegment =0;
	int whiteCount = 0;
	int pixelCount = 0;

    while (x1 != x2 || y1 != y2)
    {
        if (0 <= x1 && x1 < s->w && 0 <= y1 && y1 < s->h)
        {
			pixelCount++;
			int offset = y1 * s->pitch + x1 * format->BytesPerPixel;
			SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
				&gray, &gray, &gray);
			if(gray){
				whiteCount++;
			 	if(!whitePreviously){
					whitePreviously = 1;
					whiteSegment++;
					if(whiteSegment > segmentThreshold) return 0;
				}
			}
			else{
				whitePreviously = 0;
			}
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

	if((double)whiteCount/pixelCount<0.4) return 0;
	return 1;
}

void RemoveFalseLines(List* l , SDL_Surface* surface, int dilateStrength, int segmentThreshold){
    SDL_Surface* s = CopySurface(surface);
	s = IMGA_Dilate(s,dilateStrength);
    //IMG_SavePNG(s, "rmMe.png");

	Node* curr = l->head;
	while( curr&& l->length >1 && !CheckLine(((Line*)curr->data),s,segmentThreshold))
	{
	    //printf("Removed line\n");
		Tail(l);
		curr = l->head;
	}
	if(curr){
		while( curr->next && l->length >1){
			if (!CheckLine((Line*)curr->next->data,s, segmentThreshold)){
				//printf("Removed line\n");
				RemoveNextNode(l,curr);
			}
			else curr =curr->next;
		}
	}
    SDL_FreeSurface(s);
}