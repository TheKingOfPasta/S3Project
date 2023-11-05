#include "grid_detection.h"

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

Quadrilateral* Find_Grid(SDL_Surface *s ){
	List* lLine = HoughLine(s);
	List* l = HoughLine(s);
	printf("Hough transform done\n");
	if(l == NULL){
		errx(EXIT_FAILURE,"no lines found\n");
	}
	//printList(lLine,1);
	//DrawLines(s,lLine,255,0,0);

	Prune(lLine);
	ExcludeBorder(lLine, s->w, s->h, 0.015);
	LineFiltering(lLine,10);

	if(l->length >125){
		errx(EXIT_FAILURE,"too many lines found -> preprocessing is at fault\n");
	}
	if(l->length <4){
		errx(EXIT_FAILURE,"not enough lines found\n");
	}
	//printList(lLine,1);

	//printf("prune done\n");
	DrawLines(s,lLine,0,255,255);

	List* lquad =  FindSquares(lLine,s->w,s->h);
	if (lquad->length ==0)
	{
		errx(EXIT_FAILURE,"no quadrilateral found\n");
	}

	printf("found quad list\n");
	//printList(lLine,0);

	Node* curr = lquad->head;
	while (curr)
	{
		DrawSquare(s, curr->data, 0, 255, 0);
		curr = curr->next;
	}
	Quadrilateral *bestSquare = BestSquare(lquad);

	curr = lquad->head;

	printf("found grid !\n");
	DrawSquare(s, bestSquare, 255, 0, 255);

	Quadrilateral *grid = malloc(sizeof(Quadrilateral));
	grid->p1 =bestSquare->p1;
	grid->p2 =bestSquare->p2;
	grid->p3 =bestSquare->p3;
	grid->p4 =bestSquare->p4;
	FreeList(lLine);
	FreeList(lquad);
	return grid;
}

/*
int main(int argc, char** argv)
{
	if (argc != 3)
		errx(1,"first param : path_in\nsecond param : path_out\n");
	SDL_Surface* input = load_image(argv[1]);

	Quadrilateral* grid = Find_Grid(input);

	printf("grid found !\n");
	printQuad(grid);

	DrawSquare(input,grid,255,0,0);
	IMG_SavePNG(input,"output.png");

	SDL_FreeSurface(input);

	return 0;
}
*/
