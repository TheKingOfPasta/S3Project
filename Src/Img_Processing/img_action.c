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
        "\t\t   -p >>> perspective correction\n"
        "\t\t   you can specify a path in and path out\n");

}

#define perspective(x) (x== 'p')
#define detection(x) ((x=='d') || perspective(x))
#define sobel(x) ((x=='s') || detection(x))
#define threshold(x) (x=='t' || sobel(x))
#define blur(x) (x=='b' || threshold(x))
#define grayscale(x) (x=='g' || blur(x))

void process(char* path_in, char* path_out, char param){
    Quadrilateral* grid = NULL;

    printf("Attempting to apply all from %s\n", path_in);

    SDL_Surface* s = IMG_Load(path_in);

    const int new_width = 1000;

    int new_height = ceil((float)s->h * (float)new_width / s->w);

    s = downscale_resize(s, new_width, new_height);
    printf("Resized width from %ix%i to %ix%i\n",
            s->w, s->h, new_width, new_height);

    s= IMGC_Grayscale(s);
    //s= IMGC_Gamma_Correction(s,128);
    //s= IMGC_Contrast_Correction(s,60);
    s= IMGC_Normalize_Brigthness(s);
    s = IMGC_Level_Colors(s,10);

    if(!blur(param))goto save;
    s=IMGA_GaussianBlur(s,3,1.5);
    //s= IMGC_Contrast_Correction(s,60);
    //s= IMGC_Gamma_Correction(s,128);
    //s= IMGA_Erode(s,9);
    //s= IMGA_Dilate(s,9);

    //s = IMGC_Level_Colors(s,10);

    if(!threshold(param)) goto save;
    s=IMGA_Sauvola(s,10,0.27);
    //s=IMGA_ApplyThreshold(s,AdaptiveThreshold,Splitsize);
    s = CheckInvert(s);
    s= IMGA_Erosion(IMGA_Erosion(s));

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
    Quadrilateral* g = malloc(sizeof(Quadrilateral));
    g->p1.x = 0;
    g->p1.y = 0;
    g->p2.x = s->w;
    g->p2.y = 0;
    g->p3.x = 0;
    g->p3.y = s->h;
    g->p4.x = s->w;
    g->p4.y = s->h;
    if (grid) {
        int tempx = grid->p4.x;
        int tempy = grid->p4.y;
        grid->p4.x = grid->p3.x;
        grid->p4.y = grid->p3.y;
        grid->p3.x = tempx;
        grid->p3.y = tempy;
        s = CorrectImage(s, grid);
        printf("fixed perspective\n");
    }
    
    
    free(g);
    goto save;
    save:
    IMG_SavePNG(s, path_out);
    SDL_FreeSurface(s);
    printf("Successfully saved the new image at path %s\n", path_out);
    if (grid)  free(grid);
}

int betterMain(char param, char one)
{

    char* path_in;
    char* path_out;
    if(! grayscale(param))
        ErrorMessage();

    for(int i =1 ; i<7;i++){
        if (one != '\0' && one -'0' != i)  continue;
        if (asprintf(&path_in,"../../test_grid/image_0%i.jpeg",i) == -1)
            errx(EXIT_FAILURE, "asprintf()");
        if (asprintf(&path_out,"./sudoku0%i.png",i)==-1)
            errx(EXIT_FAILURE, "asprintf()");

        process(path_in,path_out,param);
        free(path_in);
        free(path_out);
    }
    return 1;
}

void extractProcess(char* path_in, char* path_out){
     printf("Attempting to extract from %s\n", path_in);

    SDL_Surface* s = IMG_Load(path_in);

    s = DigitExtraction (s);

    IMG_SavePNG(s, path_out);
    SDL_FreeSurface(s);

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

    char param = argv[1][1];

    char* path_in = argv[2];
    char* path_out = argv[3];

    if(param == 'e'){
        extractProcess(path_in,path_out);
        return EXIT_SUCCESS;
    }

    process(path_in,path_out,param);

    return EXIT_SUCCESS;
}
