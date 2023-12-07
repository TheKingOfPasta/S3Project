#include "Img_Processing/img_action.h"

//1 -> a==b
//0 -> a!=b
int CompareStrings(char* a, char* b)
{
    for (;*a &&*b && *a == *b; a++,b++){}
    return *a==*b;
}

void ErrorMessage()
{
    errx(EXIT_FAILURE, "Usage: -g >>> grayscale\n"
           "\t\t   -b >>> blur\n"
           "\t\t   -t >>> threshold\n"
           "\t\t   -s >>> sobel\n"
           "\t\t   -d >>> grid detection\n"
           "\t\t   -p >>> perspective correction\n");

}

#define perspective(x) (x== 'p')
#define detection(x) ((x=='d') || perspective(x))
#define sobel(x) ((x=='s') || detection(x))
#define threshold(x) (x=='t' || sobel(x))
#define blur(x) (x=='b' || threshold(x))
#define grayscale(x) (x=='g' || blur(x))

int betterMain(char param, char one)
{

    char* path_in;
    char* path_out;
    if(! grayscale(param))
        ErrorMessage();

    for(int i =1 ; i<7;i++){
        if (one != '\0' && one -'0' != i)  continue;
        Quadrilateral* grid = NULL;
        if (asprintf(&path_in,"../../test_grid/image_0%i.jpeg",i) == -1)
            errx(EXIT_FAILURE, "asprintf()");
        if (asprintf(&path_out,"./sudoku0%i.png",i)==-1)
            errx(EXIT_FAILURE, "asprintf()");

        printf("Attempting to apply all from %s\n", path_in);

        SDL_Surface* s = IMG_Load(path_in);
        const int new_width = 1000;


        int new_height = ceil((float)s->h * (float)new_width / s->w);

        s = downscale_resize(s, new_width, new_height);
        printf("Resized width from %ix%i to %ix%i\n",
                s->w, s->h, new_width, new_height);

        int sizeFactor = s->w*s->h / 100000;

        s= IMGC_Grayscale(s);
        //s = IMGC_Level_Colors(s,10);
       // s= IMGC_Gamma_Correction(s,128);
        s= IMGC_Contrast_Correction(s,32);
        s= IMGC_Normalize_Brigthness(s);

        if(!blur(param))goto save;
        s=IMGA_GaussianBlur(s,5, 1);
        //s= IMGA_Erode(s,9);
        //s= IMGA_Dilate(s,9);

        //s = IMGC_Level_Colors(s,10);

        if(!threshold(param)) goto save;
        s=IMGA_Sauvola(s,sizeFactor,0.27);
        //s=IMGA_ApplyThreshold(s,AdaptiveThreshold,Splitsize);
        s= IMGA_Erosion(CheckInvert(s));

        if(!sobel(param))goto save;
        s = sobel_gradient(s);
        if(!detection(param)) goto save;

        grid = Find_Grid(s);
        if(!grid) {
            printf("grid not found :(\n");
            goto save;
        }
        printQuad(grid);

        if (!perspective(param)) goto save;
        printf("attempt at fixing perspective\n");
        s = CorrectImage(s,grid);
        printf("fixed perspective\n");

        goto save;
        save:
        IMG_SavePNG(s, path_out);
        SDL_FreeSurface(s);
        printf("Successfully saved the new image at path %s\n", path_out);
        free(path_in);
        free(path_out);
        if (grid)  free(grid);
    }

    return 1;
}



int main(int argc, char** argv)
{
    if (argc == 1)
        ErrorMessage();

    if(argc == 2)
    {
        betterMain(argv[1][1],argv[1][2]);
        return 1;
    } //ff whats below

    SDL_Surface* sur = IMG_Load(argv[1]);
    printf("Attempting to rotate from %s\n", argv[1]);
    sur = IMGA_Rotate(sur, argc==3 ? 10 : atoi(argv[3]));
    printf("rotated done \n");
    IMG_SavePNG(sur, argv[2]);
    SDL_FreeSurface(sur);

        return EXIT_SUCCESS;
}
