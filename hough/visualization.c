#include "visualization.h"

void MyDrawLine(SDL_Surface* s, int x1, int y1, int x2, int y2,
	int r, int g, int b)
{
	/*
	printf("trying to draw : w=%i h=%i |x0 : %i  y0 : %i  x1 : %i  y1 : %i\n"
		 ,s->w,s->h,x1,y1,x2,y2);
	*/

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

void Visualize_Acc(int **acc , int size, int maxacc)
{
	SDL_Surface* newS = SDL_CreateRGBSurface(0, size, size, 32,0,0,0,0);
	SDL_LockSurface(newS);
	Uint32* pixels = (Uint32*)(newS->pixels);
	for (int i = 0; i < newS->w; i++)
    for (int j = 0; j < newS->h; j++)
	{
		Uint8 value = (Uint8) ((((float)acc[j][i])/maxacc)*255);
		pixels[i+j*newS->w] = SDL_MapRGB(newS->format, value , value, value);
	}
	SDL_UnlockSurface(newS);
	IMG_SavePNG(newS,"accumulator.png");
	SDL_FreeSurface(newS);
}

SDL_Surface* DrawLines(SDL_Surface* s,List* list,int r,int g,int b)
{
	Node* curr = list->head;
	int diag_len = sqrt(s->w * s->w + s->h * s->h);

	SDL_LockSurface(s);
	while (curr)
	{
		Line line = *(Line*)(curr->data);
		double co = (cos((line.theta)));
		double si = (sin((line.theta)));

		int x0 = line.rho * co;
		int y0 = line.rho * si;

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

void DrawSquare(SDL_Surface* s, Quadrilateral *q,int r,int g,int b)
{
	SDL_LockSurface(s);
    MyDrawLine(s, q->p1.x , q->p1.y , q->p2.x , q->p2.y ,r,g,b);
    MyDrawLine(s, q->p2.x , q->p2.y , q->p3.x , q->p3.y ,r,g,b);
    MyDrawLine(s, q->p3.x , q->p3.y , q->p4.x , q->p4.y ,r,g,b);
    MyDrawLine(s, q->p4.x , q->p4.y , q->p1.x , q->p1.y ,r,g,b);
	SDL_UnlockSurface(s);
}

void printQuad(Quadrilateral* q){
	printf("	square pointer %p\n",q);
	printf("	p1 x: %3i  y: %3i \n",q->p1.x,q->p1.y);
	printf("	p2 x: %3i  y: %3i \n",q->p2.x,q->p2.y);
	printf("	p3 x: %3i  y: %3i \n",q->p3.x,q->p3.y);
	printf("	p4 x: %3i  y: %3i \n",q->p4.x,q->p4.y);
}

void printList(List* l, int line)
{
	if(line){
		printf("line list = \n");
		Node* temp = l->head;
		if(!temp) printf("empty\n");
		int i =1;
		while(temp){
			Line line = *(Line*)(temp->data);
			printf("%3i : theta %2.3f (deg %i) rho %5f\n",i,line.theta,
					 (int)((line.theta)*180/M_PI),line.rho);
			i++;
			temp = temp->next;
		}
	}
	else{
		printf("line list = \n");
		Node* temp = l->head;
		if(!temp) printf("empty\n");
		int i =1;
		while(temp){
			printf("%3i : ",i);
			printQuad((Quadrilateral*)(temp->data));
			i++;
			temp = temp->next;
		}
	}
}

int FindIntersectio(Line* l1 ,Line* l2, int w, int h, int *x, int *y ){
	//lines are not parallel
	float cost1=cosf(ToRad(l1->theta));
    float cost2=cosf(ToRad(l2->theta));
    float sint1=sinf(ToRad(l1->theta));
    float sint2=sinf(ToRad(l2->theta));
    float a=cost1*sint2-sint1*cost2;
    *x = (int)((sint2*l1->rho - sint1*l2->rho)/a);
	*y = (int)((cost1*l2->rho - cost2*l1->rho)/a);
	//check if the intersection is not out of bound
	return *x >0 && *y >0 && *x<w && *y<h;
}

void DrawIntersections(SDL_Surface* s,List* l){

	Node *curr = l->head;
	int index = 0;

	Point p;
	Uint32* pixels = (Uint32*)(s->pixels);
	SDL_LockSurface(s);
	while(curr){
		double angle = (((Line*)(curr->data))->theta) + M_PI_2;
		Node *innerCurr = l->head;
		int innerIndex =0;
		while(innerCurr){
			if(CloseAngle(angle,((Line*)(innerCurr->data))->theta,ToRad(30)) &&
			 	FindIntersectio(((Line*)(curr->data)),
					((Line*)(innerCurr->data)),
					s->w,
					s->h,
					&(p.x),
					&(p.y)))
				{
				pixels[p.x + p.y *s->w] = SDL_MapRGB(s->format, 255, 0, 0);
				pixels[p.x-1 + p.y *s->w] = SDL_MapRGB(s->format, 255, 0, 0);
				pixels[p.x+1 + p.y *s->w] = SDL_MapRGB(s->format, 255, 0, 0);
				pixels[p.x + (p.y+1) *s->w] = SDL_MapRGB(s->format, 255, 0, 0);
				pixels[p.x + (p.y-1) *s->w] = SDL_MapRGB(s->format, 255, 0, 0);
			}
			innerIndex++;
			innerCurr= innerCurr->next;
		}
		curr = curr->next;
		index++;
	}
	SDL_UnlockSurface(s);
}
