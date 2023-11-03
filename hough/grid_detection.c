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
		printf("Hough transform done\n");
		//printList(lLine);
	Prune(lLine);
		printf("prune done\n");
		printList(lLine,1);
		IMG_SavePNG(DrawLines(s,lLine,0,255,255), "lines.png");

	List* lquad =  FindSquares(lLine,s->w,s->h);
		printf("found quad list\n");
		printList(lLine,0);
		IMG_SavePNG(DrawLines(s,lLine,0,255,0), "quads.png");


	Quadrilateral * grid = BestSquare(lquad);

	FreeList(lLine);
	return grid;
}


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
