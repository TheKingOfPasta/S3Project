#include "img_action.h"

//1 -> a==b
//0 -> a!=b
int CompareStrings(char* a, char* b)
{
    for (;*a &&*b && *a == *b; a++,b++){}
    return *a==*b;
}

void ErrorMessage()
{
    errx(EXIT_FAILURE, "Usage: -r/--rotate\n"
           "                   -ra/--rotateauto\n"
           "                   -b/--blur\n"
           "                   -t/--threshold\n"
           "                   -s/--sobel\n"
           "                   -i/--inverse\n"
           "                   -w/--wrapping>wrapping\n"
	   "		       -bt/--blur>threshold\n"
           "                   -bts/--blur>threshold>sobel\n"
           "                   -a/--all\n"
           "                   -g/--grayscale\n"
           "                   -agd/--all>grid_detect\n");
}

#define detection(x) (x=='d')
#define sobel(x) ((x=='s') || detection(x))
#define threshold(x) (x=='t' || sobel(x))
#define blur(x) (x=='b' || threshold(x))
#define grayscale(x) (x=='g' || blur(x))


void normalize(SDL_Surface * s){
    Uint8 r,g,b;
    Uint8 m =0;

    Uint32 * pixels = s->pixels;
    for (int i = 0; i < s->w; i ++)
    for (int j = 0; j < s->h; j ++)
    {
        SDL_GetRGB(pixels[i + (j) * s->w],
                    s->format,
                    &r,&g,&b);
        if (r>m) m= r;
    }

    for (int i = 0; i < s->w; i ++)
    for (int j = 0; j < s->h; j ++)
    {
        SDL_GetRGB(pixels[i + (j) * s->w],
                    s->format,
                    &r,&g,&b);
        Uint8 a = r * (255.0/m);
        pixels[i+j*s->w] = SDL_MapRGB(s->format, a, a, a);
    }
}


int betterMain(char param, char one)
{

    char* path_in;
    char* path_out;
    if(! grayscale(param))
    {
        errx(EXIT_FAILURE, "Usage: -g >>> grayscale\n"
           "\t\t   -b >>> blur\n"
           "\t\t   -t >>> threshold\n"
           "\t\t   -s >>> sobel\n"
           "\t\t   -d >>> grid detection\n");
    }

    for(int i =1 ; i<7;i++){
        if (one != '\0' && one -'0' != i)  continue;
        if (asprintf(&path_in,"../test_grid/image_0%i.jpeg",i) == -1)
            errx(EXIT_FAILURE, "asprintf()");
        if (asprintf(&path_out,"./sudoku0%i.png",i)==-1)
            errx(EXIT_FAILURE, "asprintf()");

        printf("Attempting to apply all from %s\n", path_in);
        SDL_Surface* s = IMG_Load(path_in);
        int AdaptiveFactor = s->w*s->h ;
        //printf("adaptive factor : %i\n",AdaptiveFactor);

        s= IMGC_surface_to_grayscale(s);

        if(!blur(param))goto save;
        s=IMGA_GaussianBlur(s,Blursize, BlurIntensity);

        if(!threshold(param)) goto save;
        //s=IMGA_Erosion(CheckInvert(IMGA_AdaptiveThresholdDeluxe(s,Splitsize)));
        //s=IMGA_Erosion(CheckInvert(IMGA_OtsuThreshold(s)));
        s=IMGA_Erosion(CheckInvert(IMGA_Sovela(s,3,0.2)));
        //s=IMGA_Erosion(CheckInvert(IMGA_ApplyThreshold(s,AdaptiveThreshold,Splitsize)));

        if(!sobel(param))goto save;
        s = sobel_gradient(s);
        if(!detection(param)) goto save;
        Quadrilateral* grid = Find_Grid(s);
        if(!grid) printf("not found grid :(\n");
        else printQuad(grid);
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


    char* path_in = argv[2];
    char* path_out = argv[3];

    if (CompareStrings(argv[1], "-b") ||
                CompareStrings(argv[1], "--blur"))
    {
        int size;
        double sigma;

        if (argc == 6)
        {
            char* endptr;
            size = atoi(argv[4]);
            sigma = strtod(argv[5], &endptr);
        }
        else if (argc == 5)
        {
            size = atoi(argv[4]);
            sigma = BlurIntensity;
        }
        else if (argc == 4)
        {
            size = Blursize;
            sigma = BlurIntensity;
        }
        else
            errx(EXIT_FAILURE, "-b/--blur : gaussian blur "
                    "(path_in path_out [size, sigma])\n");

        printf("Attempting to blur image from %s\n", path_in);
        IMG_SavePNG(
            IMGA_GaussianBlur(
                IMG_Load(path_in),
                size,
                sigma),
            path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-w") ||
                CompareStrings(argv[1], "--wrapping"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-w/--wrapping : wrapping at path "
                        "(path_in path_out)\n");

	/*Quadrilateral* quad = malloc(sizeof(Quadrilateral));
	Point p1;
	p1.x = 0;
	p1.y = 0;
	Point p2;
	p2.x = 0;
	p2.y = 40;
	Point p3;
	p3.x = 40;
	p3.y = 40;
	Point p4;
	p4.x = 40;
	p4.y = 0;
	quad->p1 = p1;
	quad->p2 = p2;
	quad->p3 = p3;
	quad->p4 = p4;*/
        SDL_Surface* s = IMG_Load(path_in);
        s = IMGC_surface_to_grayscale(s);
        s = sobel_gradient(IMGA_Erosion(CheckInvert(
                IMGA_ApplyThreshold(
                        IMGA_GaussianBlur( s,Blursize,BlurIntensity),
                                AdaptiveThreshold,Splitsize))));

        Quadrilateral* grid = Find_Grid(s);
	printQuad(grid); 
	printf("Attempting to wrap image from %s\n",path_in);
	SDL_Surface* wrapped = Wrapping(path_in, grid);
	printf("Attemption to save image\n");
	IMG_SavePNG(wrapped,path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }

    else if (CompareStrings(argv[1], "-r") ||
                CompareStrings(argv[1], "--rotate"))
    {
        if (argc != 5)
            errx(EXIT_FAILURE, "-r/--rotate : rotate at path "
                        "(path_in path_out angle)\n");

        char* endptr;
        double angle = strtod(argv[4], &endptr);

        printf("Attempting to rotate image from %s\n", path_in);
        IMG_SavePNG(IMGA_Rotate_from(path_in, angle), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-ra") ||
                CompareStrings(argv[1], "--rotateauto"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-ra/--rotateauto : rotate auto at path "
                        "(path_in path_out)\n");

        printf("Attempting to automatically rotate image from %s\n", path_in);
        IMG_SavePNG(IMGA_Rotate_auto(path_in), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-t") ||
                CompareStrings(argv[1], "--threshold"))
    {
        if (argc == 4)
        {
            printf("Attempting to apply thresholding to %s\n", path_in);
            IMG_SavePNG(IMGA_ApplyThreshold(
                            IMG_Load(path_in),
                            AdaptiveThreshold,Splitsize),
                        path_out);
            printf("Successfully saved the new image at path %s\n", path_out);
            return EXIT_SUCCESS;
        }

        if (argc != 5 && argc != 6)
            errx(EXIT_FAILURE, "-t/--threshold : apply adaptive thresholding "
                        "(path_in path_out [threshold] [m]) "
                        "(m (optional) : apply thresholds from 0 to n)\n");


        char* endptr;
        double threshold = strtod(argv[4], &endptr);

		char* str;

        printf("Attempting to apply thresholding image from %s\n", path_in);
		if (argc == 5)
        {
			IMG_SavePNG(
                IMGA_ApplyThreshold(
                    IMG_Load(path_in),
                    threshold, Splitsize),
                path_out);
        }
		else
            for (int i = 0; i <= threshold; i++)
            {
                if(asprintf(&str, "%s/thresholded_%i.png", path_out, i) == -1)
                    err(EXIT_FAILURE,"asprintf()");
                IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), i, Splitsize), str);
            }
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-s") ||
                CompareStrings(argv[1], "--sobel"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-s/--sobel : apply sobel edge detection "
                        "(path_in path_out)\n");

        printf("Attempting to sobel image from %s\n", path_in);
        IMG_SavePNG(sobel_gradient(IMG_Load(path_in)), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-bts") ||
                CompareStrings(argv[1], "--blur>threshold>sobel"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-bts/--blur>threshold>sobel : "
                "apply blur>threshold>sobel (path_in path_out)\n");

        printf("Attempting to blur>threshold>sobel image from %s\n", path_in);
        IMG_SavePNG(sobel_gradient(IMGA_Erosion(IMGA_ApplyThreshold(
                                                    IMGA_GaussianBlur(
                                                        IMG_Load(path_in),
                                                        Blursize,
                                                        BlurIntensity),
                                                    AdaptiveThreshold,Splitsize))),
                    path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-bt") ||
                CompareStrings(argv[1], "--blur>threshold"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-bt/--blur>threshold : apply blur>threshold "
                        "(path_in path_out)\n");

        printf("Attempting to blur>threshold image from %s\n", path_in);
        IMG_SavePNG(IMGA_Erosion(IMGA_ApplyThreshold(IMGA_GaussianBlur(
                                                        IMG_Load(path_in),
                                                        Blursize,
                                                        BlurIntensity),
                                                    AdaptiveThreshold,Splitsize)),
                path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-i") ||
                CompareStrings(argv[1], "--invert"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-i/--invert : inverts the colors of the image"
                "only if it needs to be inverted at path"
                "(path_in path_out)\n");

        printf("Attempting to apply invert at %s\n", path_in);
        IMG_SavePNG(CheckInvert(IMG_Load(path_in)), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-a") ||
                CompareStrings(argv[1], "--all"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-a/--all : applies every function "
                "(path_in path_out)\n");

        printf("Attempting to apply all from %s\n", path_in);
        SDL_Surface* s = IMG_Load(path_in);
        s = IMGC_surface_to_grayscale(s);
        IMG_SavePNG(
            sobel_gradient(IMGA_Erosion(CheckInvert(IMGA_ApplyThreshold(
                                                        IMGA_GaussianBlur(
                                                            s,
                                                            Blursize,
                                                            BlurIntensity),
                                                        AdaptiveThreshold,Splitsize)))),
            path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-g") ||
                CompareStrings(argv[1], "--grayscale"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-g/--grayscale : applies a grayscale filter "
                "(path_in path_out)\n");

        printf("Attempting to apply grayscale to %s\n", path_in);
        SDL_Surface* s = IMGC_surface_to_grayscale(IMG_Load(path_in));
	IMG_SavePNG(s,path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-agd") ||
                CompareStrings(argv[1], "--all>grid_detect"))
    {
        if (argc ==2)
        {
            for(int i =1 ; i<7;i++){
                if(asprintf(&path_in,"../test_grid/sudoku0%i.png",i) == -1)
                    err(EXIT_FAILURE,"asprintf()");
                if (asprintf(&path_out,"./sudoku0%i.png",i)==-1)
                    err(EXIT_FAILURE,"asprintf()");
                printf("Attempting to apply all from %s\n", path_in);
                SDL_Surface* s = IMG_Load(path_in);
                s = IMGC_surface_to_grayscale(s);
                s = sobel_gradient(IMGA_Erosion(CheckInvert(
                        IMGA_ApplyThreshold(
                            IMGA_GaussianBlur(s,Blursize, BlurIntensity),
                                            AdaptiveThreshold,Splitsize))));
                Quadrilateral* grid = Find_Grid(s);
                IMG_SavePNG(s, path_out);
                if(!grid){
                    printf("not found grid :(\n");
                    continue;
                }
                printQuad(grid);
                printf("Successfully saved the new image at path %s\n", path_out);
            }
            return 1;
        }

        if (argc != 4)
            errx(EXIT_FAILURE, "-agd/--all>grid_detect : "
                "applies every function then "
                "applies the grid detection algorithm(s) "
                "(path_in path_out)\n");

        printf("Attempting to apply all from %s\n", path_in);
        SDL_Surface* s = IMG_Load(path_in);
        s = IMGC_surface_to_grayscale(s);

        s = sobel_gradient(IMGA_Erosion(CheckInvert(
                IMGA_ApplyThreshold(
                        IMGA_GaussianBlur( s,Blursize,BlurIntensity),
                                AdaptiveThreshold,Splitsize))));

        Quadrilateral* grid = Find_Grid(s);
        IMG_SavePNG(s, path_out);
        if(!grid)
            printf("not found grid :(\n");
        else{
            printQuad(grid);
            printf("Successfully saved the new image at path %s\n", path_out);
        }
    }
    else if (CompareStrings(argv[1], "-ho") ||
                CompareStrings(argv[1], "--all>hough"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-agd/--all>grid_detect : "
                "applies every function then "
                "applies the grid detection algorithm(s) "
                "(path_in path_out)\n");

        printf("Attempting to apply all from %s\n", path_in);
        SDL_Surface* s = IMG_Load(path_in);
        Quadrilateral* grid = Find_Grid(s);
        IMG_SavePNG(s, path_out);
        if(!grid)
            printf("not found grid :(\n");
        else{
            printQuad(grid);
            printf("Successfully saved the new image at path %s\n", path_out);
        }
    }
    else
        ErrorMessage();

    return EXIT_SUCCESS;
}
