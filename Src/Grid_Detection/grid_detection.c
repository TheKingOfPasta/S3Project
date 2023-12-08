#include "Grid_Detection/grid_detection.h"

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

Quadrilateral* Find_Grid(SDL_Surface *s )
{
    List* lLine = HoughLine(s);
    printf("Hough transform done\n");

    //DrawLines(s,lLine,50,0,0);
    AveragesCloseLine(lLine,ceil(sqrt(s->w * s->w + s->h * s->h)));


    ExcludeBorderLine(lLine, s->w, s->h, 0.015);
    List* LVer = InitList();
	List* LHor = InitList();

    RemovesStrayLine(lLine,7,LVer,LHor);
    FreeList(lLine);

   // printf("LVERRRRRR\n");
    RemoveFalseLines(LVer,s);
   // printf("LHORRRRRR\n");
    RemoveFalseLines(LHor,s);

	// printf("vertical Line list\n");
    // printList(LVer,1);
	// printf("horizontal Line list\n");
	// printList(LHor,1);


    DrawLines(s,LVer,255,0,0);
    DrawLines(s,LHor,0,0,255);

    int nbLines = LVer->length + LHor->length;
    if(nbLines >150){
        printf("too many lines found -> preprocessing is at fault\n");
        FreeList(LVer);
        FreeList(LHor);
        return NULL;
    }
    if(nbLines<4){
        printf("not enough lines found\n");
        FreeList(LVer);
        FreeList(LHor);
        return NULL;
    }

    List* lquad =  FindSquares(LVer,LHor,s->w,s->h);
    FreeList(LVer);
    FreeList(LHor);

    if (lquad->length ==0)
    {
        FreeList(lquad);
        printf("no quadrilateral found\n");
        return NULL;
    }



    //printf("found quad list\n");
    //printList(lLine,0);

    Node* curr = lquad->head;
    while (curr)
    {
        DrawSquare(s, curr->data, 0, 255, 0,0);
        curr = curr->next;
    }

    Quadrilateral *bestSquare = BestSquare(lquad);
    //printf("found grid !\n");
    DrawSquare(s, bestSquare, 255, 0, 255,1);

    Quadrilateral *grid = malloc(sizeof(Quadrilateral));
    grid->p1 =bestSquare->p1;
    grid->p2 =bestSquare->p2;
    grid->p3 =bestSquare->p3;
    grid->p4 =bestSquare->p4;
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
