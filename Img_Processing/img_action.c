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
           "\t\t   -r >>> rotate\n"
           "\t\t   -w >>> wrapping\n");

}

#define wrapping(x) (x == 'w')
#define rotate(x) ((x== 'r') || wrapping(x))
#define detection(x) ((x=='d') || rotate(x))
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
        if (asprintf(&path_in,"../test_grid/image_0%i.jpeg",i) == -1)
            errx(EXIT_FAILURE, "asprintf()");
        if (asprintf(&path_out,"./sudoku0%i.png",i)==-1)
            errx(EXIT_FAILURE, "asprintf()");

        printf("Attempting to apply all from %s\n", path_in);
        SDL_Surface* sur = IMG_Load(path_in);

        const int new_width = 1000;

        int new_height = (int)ceil(sur->h * (float)new_width / sur->w);
        SDL_Surface *s = downscale_resize(sur, new_width, new_height);
        printf("Resized width from %ix%i to %ix%i\n",
                sur->w, sur->h, new_width, new_height);

        int sizeFactor = s->w*s->h / 100000;

        s= IMGC_Grayscale(s);
        //s = IMGC_Level_Colors(s,10);
        s= IMGC_Gamma_Correction(s,128);
        s= IMGC_Contrast_Correction(s,64);
        //s = IMGC_Normalize_Brigthness(s);

        if(!blur(param))goto save;
        s=IMGA_GaussianBlur(s,3, 1);
        s= IMGA_Erode(s,9);
        s= IMGA_Dilate(s,9);

        //s = IMGC_Level_Colors(s,10);

        if(!threshold(param)) goto save;
        s=IMGA_Erosion(IMGA_Sauvola(s,sizeFactor,0.25));
        //s=IMGA_Erosion(CheckInvert(IMGA_ApplyThreshold(s,AdaptiveThreshold,Splitsize)));

        if(!sobel(param))goto save;
        s = sobel_gradient(s);
        if(!detection(param)) goto save;

        Quadrilateral* grid = Find_Grid(s);
        if(!grid) {
            printf("grid not found :(\n");
            goto save;
        }
        printQuad(grid);

        if (!rotate(param)) goto save;

        int angle = FindAngle(grid,s->w,s->h);
        printf("Attempting to rotate : angle %i\n",angle);
        s = IMGA_Rotate(s, angle);
        printf("rotating done \n");
        // if (asprintf(&path, "rotated_0%i.png", i) == -1)
        //     errx(EXIT_FAILURE, "asprintf()");
        // IMG_SavePNG(s, path);
        // printf("     saved at %s\n", path);

        if(!wrapping(param)) goto save;
        printf("Attempting to Wrap\n");
        s = WrappingSurface(s, grid);
        printf("wrapping done \n");

        goto save;
        save:
        IMG_SavePNG(s, path_out);
        SDL_FreeSurface(s);
        printf("Successfully saved the new image at path %s\n", path_out);
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
