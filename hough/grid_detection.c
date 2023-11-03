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


int main(int argc, char** argv)
{
	if (argc != 3)
		errx(1,"first param : path_in\nsecond param : path_out\n");
	SDL_Surface* input = load_image(argv[1]);


	ListLine* list = HoughLine(input);
	printf("Hough transform done\n");
	//printList(list);
	Prune(list);
	printf("prune done\n");
	printList(list);

	ListToSurface(input,list,255,0,0);
	IMG_SavePNG(ListToSurface(input,list,0,255,255), "lines.png");

	ListQuad* lquad =  FindSquares(list,input->w,input->h);
	NodeQuadrilateral * grid = BestSquare(lquad);
	printf("square found !\n");
	printQuad(grid);
	DrawSquare(input,grid,255,0,0);
	IMG_SavePNG(input, argv[2]);

	SDL_FreeSurface(input);
	FreeList(list);
	return 0;
}
