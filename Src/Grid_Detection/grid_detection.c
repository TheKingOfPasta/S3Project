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

Quadrilateral* Find_Grid(SDL_Surface **s )
{
    List* lLine = HoughLine(*s);
    printf("Hough transform done\n");
    int w = (*s)->w;
    int h = (*s)->h;

    AveragesCloseLine(lLine,ceil(sqrt(w * w + h *h)),5, 0.015);
    //DrawLines(*s,lLine,100,0,0);


    ExcludeBorderLine(lLine, w, h, 0.015);
    List* LVer = InitList();
	List* LHor = InitList();

    RemovesStrayLine(lLine,7,LVer,LHor);
    FreeList(lLine);

   // printf("LVERRRRRR\n");
    RemoveFalseLines(LVer,*s,9,9);
   // printf("LHORRRRRR\n");
    RemoveFalseLines(LHor,*s,9,9);

	// printf("vertical Line list\n");
    // printList(LVer,1);
	// printf("horizontal Line list\n");
	// printList(LHor,1);


    DrawLines(*s,LVer,255,0,0);
    DrawLines(*s,LHor,0,0,255);

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

    double paddingPercentage = 10;

    List* lquad =  FindSquares(LVer,LHor,w,h,paddingPercentage);
    FreeList(LVer);
    FreeList(LHor);

    if (lquad->length ==0)
    {
        FreeList(lquad);
        printf("no quadrilateral found\n");
        return NULL;
    }

    // w = (*s)->w;
    // h = (*s)->h;

    //printf("found quad list\n");
    //printList(lLine,0);

    // Node* curr = lquad->head;
    // while (curr)
    // {
    //     DrawSquare(*s, curr->data, 0, 255, 0,0);
    //     curr = curr->next;
    // }

    *s= Padding(*s,paddingPercentage);
    Quadrilateral *bestSquare = BestSquare(lquad);
    //printf("found grid !\n");

    Quadrilateral *grid = malloc(sizeof(Quadrilateral));
    grid->p1.x =bestSquare->p1.x + w*(paddingPercentage/200);
    grid->p1.y =bestSquare->p1.y + h*(paddingPercentage/200);
    grid->p2.x =bestSquare->p2.x + w*(paddingPercentage/200);
    grid->p2.y =bestSquare->p2.y + h*(paddingPercentage/200);
    grid->p3.x =bestSquare->p3.x + w*(paddingPercentage/200);
    grid->p3.y =bestSquare->p3.y + h*(paddingPercentage/200);
    grid->p4.x =bestSquare->p4.x + w*(paddingPercentage/200);
    grid->p4.y =bestSquare->p4.y + h*(paddingPercentage/200);
    FreeList(lquad);

    DrawSquare(*s, grid, 255, 0, 255,1);

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
