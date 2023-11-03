#include "visualization.h"

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
		pixels[i+j*newS->w] = SDL_MapRGB(newS->format, value , value, value);
	}
	SDL_UnlockSurface(newS);
	IMG_SavePNG(newS,"accumulator.png");
	SDL_FreeSurface(newS);
}

SDL_Surface* ListToSurface(SDL_Surface* s,ListLine* list,int r,int g,int b)
{
	NodeLine* curr = list->head;
	int diag_len = sqrt(s->w * s->w + s->h * s->h);

	SDL_LockSurface(s);
	while (curr)
	{
		double co = DoubleAbs(cos(ToRad(curr->theta)));
		double si = DoubleAbs(sin(ToRad(curr->theta)));

		int x0 = curr->rho * co;
		int y0 = curr->rho * si;
		
		int x1 = x0 - diag_len * si;
		int y1 = y0 + diag_len * co;

		int x2 = x0 + diag_len * si;
		int y2 = y0 - diag_len * co;

		MyDrawLine(s, x1, y1, x2, y2,r,g,b);

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

void printQuad(NodeQuadrilateral* q){
	printf("	square pointer %p\n",q);
	printf("	p1 x: %3i  y: %3i \n",q->p1.x,q->p1.y);
	printf("	p2 x: %3i  y: %3i \n",q->p2.x,q->p2.y);
	printf("	p3 x: %3i  y: %3i \n",q->p3.x,q->p3.y);
	printf("	p4 x: %3i  y: %3i \n",q->p4.x,q->p4.y);
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