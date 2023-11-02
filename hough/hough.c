#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <err.h>

#define CloseAngle(t1, t2, threshold) (threshold + abs(t1 - t2) % 180) < (threshold<<1)
#define toRad(x) x*M_PI/180.0
typedef struct NodeLine{
	struct NodeLine * next;
	double rho;
	int theta;
} NodeLine;

typedef struct ListLine {
	NodeLine* head;
	int size;
} ListLine;

typedef struct Point {
	int x;
	int y;
} Point;


typedef struct NodeQuadrilateral{
	struct NodeQuadrilateral * next;
	Point p1,p2,p3,p4;
} NodeQuadrilateral;


typedef struct ListQuad {
	NodeQuadrilateral* head;
} ListQuad;




void MyDrawLine(SDL_Surface* s, int x1, int y1, int x2, int y2, int r, int g, int b)
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
            pixels[x1 + y1 * s->w] = SDL_MapRGB(s->format, r, g, b);
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

void Preppend(ListLine* l, NodeLine* nd){
	nd->next = l->head;
	l->head = nd;
	l->size++;
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
		cos_t[i + 45] = cos(toRad(i));
		sin_t[i + 45] = sin(toRad(i));
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

	unsigned int maxVal = 0;

	for (int i = 0; i < diag_len * 2; i++)
	for (int j = 0; j < 180; j++)
	{
		if (accumulator[i][j] > maxVal)
			maxVal = accumulator[i][j];
	}

	Visualize_Acc(accumulator, diag_len * 2, maxVal);

    unsigned int line_threshold = maxVal / 2;//line threshold %

    int maxTheta = 0, maxRho = 0;
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

SDL_Surface* ListToSurface(SDL_Surface* s,ListLine* list)
{
	NodeLine* curr = list->head;
	int diag_len = sqrt(s->w * s->w + s->h * s->h);

	SDL_LockSurface(s);
	while (curr)
	{
		double co = DoubleAbs(cos(toRad(curr->theta)));
		double si = DoubleAbs(sin(toRad(curr->theta)));

		int x0 = curr->rho * co;
		int y0 = curr->rho * si;
		
		int x1 = x0 - diag_len * si;
		int y1 = y0 + diag_len * co;

		int x2 = x0 + diag_len * si;
		int y2 = y0 - diag_len * co;

		MyDrawLine(s, x1, y1, x2, y2,0,255,255);

		curr = curr->next;
	}
	SDL_UnlockSurface(s);
	return s;
}

void DrawSquare(SDL_Surface* s, NodeQuadrilateral *q)
{
	SDL_LockSurface(s);
    MyDrawLine(s, q->p1.x , q->p1.y , q->p2.x , q->p2.y ,255,0,0);
    MyDrawLine(s, q->p2.x , q->p2.y , q->p3.x , q->p3.y ,255,0,0);
    MyDrawLine(s, q->p3.x , q->p3.y , q->p4.x , q->p4.y ,255,0,0);
    MyDrawLine(s, q->p4.x , q->p4.y , q->p1.x , q->p1.y ,255,0,0);
	SDL_UnlockSurface(s);
}

int FindIntersection(NodeLine* l1 ,NodeLine* l2, int w, int h,int *x, int *y ){
	//lines are not parallel
	float cost1=cosf(toRad(l1->theta));
    float cost2=cosf(toRad(l2->theta));
    float sint1=sinf(toRad(l1->theta));
    float sint2=sinf(toRad(l2->theta));
    float a=cost1*sint2-sint1*cost2;  
    *x = (int)((sint2*l1->rho - sint1*l2->rho)/a);
	*y = (int)((cost1*l2->rho - cost2*l1->rho)/a);
	//check if the intersection is not out of bound 
	return *x >0 && *y >0 && *x<w && *y<h;
}

double Min4(double a,double b,double c,double d){
	return (a < b ? (a < c ? (a < d ? a : d) : (c < d ? c : d)) :
					(b < c ? (b < d ? b : d) : (c < d ? c : d)));
}

double Max4(double a,double b,double c,double d){
	return (a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d)) :
					(b > c ? (b > d ? b : d) : (c > d ? c : d)));
}

double Dist(Point p1, Point p2)
{
    double dX = p1.x - p2.x;
    double dY = p1.y - p2.y;
    return sqrt(dX*dX + dY*dY);
}

double Area(Point p1,Point p2,Point p3,Point p4)
{
    // area of a triangle = base * height / 2
    double base = Dist(p1, p3);

    double dist2to3 = Dist(p2, p3);
    double height1 = sqrt(dist2to3 * dist2to3 - base * base / 4);
    
    double a = base * height1 / 2;//Area of the upper triangle (1,2,3)

    double dist3to4 = Dist(p3, p4);
    double height2 = sqrt(dist3to4 * dist3to4 - base * base / 4);

    a += base * height2 / 2;//Area of the lower triangle (1,3,4)
	return a;
}

double MinMaxRatio(Point p1,Point p2,Point p3,Point p4){
	double l1 = Dist(p1,p2);
	double l2 = Dist(p2,p3);
	double l3 = Dist(p3,p4);
	double l4 = Dist(p4,p1);
	double mi = Min4(l1,l2,l3,l4);
	double ma = Max4(l1,l2,l3,l4);
	
	return DoubleAbs(mi/ma-1);
}

NodeQuadrilateral* BestSquare(ListQuad *l){
	if(!l->head) err(EXIT_FAILURE,"empty quad list\n");

	NodeQuadrilateral* currQuad = l->head;
	NodeQuadrilateral* bestSquare = l->head;
	double bestRatio = 0;

	while(currQuad){
		Point p1 = currQuad->p1;
    	Point p2 = currQuad->p2;
    	Point p3 = currQuad->p3;
    	Point p4 = currQuad->p4;
		double l1 = Dist(p1,p2);
		double l2 = Dist(p2,p3);
		double l3 = Dist(p3,p4);
		double l4 = Dist(p4,p1);
		double min = Min4(l1,l2,l3,l4);
		double max = Max4(l1,l2,l3,l4);
		min = min*min;
		max = max*max;
		double areaRatio  = (max - min)/max *min;
		double selectionRatio =min * 3 - areaRatio * 10; 
		if(selectionRatio> bestRatio){
			bestRatio = selectionRatio;
			bestSquare = currQuad;
		}
		currQuad = currQuad->next;
	}
	return bestSquare;
}

void printQuad(NodeQuadrilateral* q){
	printf("	square pointer %p\n",q);
	printf("	p1 x: %3i  y: %3i \n",q->p1.x,q->p1.y);
	printf("	p2 x: %3i  y: %3i \n",q->p2.x,q->p2.y);
	printf("	p3 x: %3i  y: %3i \n",q->p3.x,q->p3.y);
	printf("	p4 x: %3i  y: %3i \n",q->p4.x,q->p4.y);
}

ListQuad* FindSquares(ListLine* l,int width, int height){
	
	Point intersection[l->size][l->size];
	
	NodeLine *curr = l->head;
	int index = 0;

	Point p;
	
	while(curr){
		int angle = curr->theta +90 %180;
		NodeLine *innerCurr = l->head;
		int innerIndex =0;
		while(innerCurr){
			int e =CloseAngle(angle,innerCurr->theta,30); 
			if( e && FindIntersection(curr,innerCurr,width,height,&(p.x),&(p.y))){
				intersection[index][innerIndex] = p; 
			}
			else {
				intersection[index][innerIndex] = (Point){x:-1,y:-1};
			}
			innerIndex++;
			innerCurr= innerCurr->next;
		}
		curr = curr->next;
		index++;
	}

	printf("----");
	for (int j = 0; j < l->size; j++)
		printf("[%2i]        ",j);
	printf("\n");			
	for (int i =0 ; i < l->size; i++)
	{
		printf("[%2i]",i);
		for (int j = 0; j < l->size; j++)
		{
			printf("x:%3i y:%3i ",intersection[i][j].x,intersection[i][j].y);
		}
		printf("\n");
	}

	ListQuad *lquad = malloc(sizeof(ListQuad));
	lquad->head = NULL;
	for(int i1 = 0; i1<l->size;i1++)
		for(int i2 = 0; i2<l->size;i2++)
			if(intersection[i1][i2].x !=-1)
				for (int i3 = 0; i3 < l->size; i3++)
					if(i3!=i1 && intersection[i2][i3].x !=-1)
						for (int i4 = 0; i4 < l->size; i4++)
							if(i4!=i2 && intersection[i3][i4].x !=-1)
								if(intersection[i4][i1].x != -1){
									NodeQuadrilateral *nd = malloc(sizeof(NodeQuadrilateral));
									nd->next = lquad->head;
									nd->p1 = intersection[i1][i2];
									nd->p2 = intersection[i2][i3];
									nd->p3 = intersection[i3][i4];
									nd->p4 = intersection[i4][i1];
									lquad->head = nd;
								}
	
	return lquad;
}

void FreeList(ListLine *l)
{
    NodeLine* curr = l->head;

    while (curr)
    {
        NodeLine* prev = curr;
        curr = curr->next;
        free(prev);
    }
    
    free(l);
}


int main(int argc, char** argv)
{
	if (argc != 3)
		errx(1,"first param : path_in\nsecond param : path_out\n");
	SDL_Surface* input = load_image(argv[1]);
	

	ListLine* list = HoughLine(input);
	Prune(list);
	printList(list);
	IMG_SavePNG(ListToSurface(input,list), "lines.png");
	printf("lines done\n");
	ListQuad* lquad =  FindSquares(list,input->w,input->h);
	NodeQuadrilateral * grid = BestSquare(lquad);
	printf("square found :\n");
	printQuad(grid);
	

	DrawSquare(input,grid);
	printf("square drown\n");
	IMG_SavePNG(input, argv[2]);

	SDL_FreeSurface(input);
	FreeList(list);
	printf("\n\nAfter prune: ");
	return 0;
}
