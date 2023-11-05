#include "img_action.h"

#define Blursize 13
#define BlurIntensity 3
#define AdaptiveThreshold 3
#define Splitsize 75000

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
           "                   -bt/--blur>threshold\n"
           "                   -bts/--blur>threshold>sobel\n"
           "                   -a/--all\n"
           "                   -g/--grayscale\n"
           "                   -agd/--all>grid_detect\n");
}

SDL_Surface* IMGA_Erosion(SDL_Surface* input){
	SDL_Surface* output =
	   	SDL_CreateRGBSurface(0,input->w,input->h,32,0,0,0,0);

    SDL_LockSurface(output);

    Uint32* inpPixels = (Uint32*)(input->pixels);
    Uint32* outPixels = (Uint32*)(output->pixels);

	int sumWhite;
	for (int i = 1; i < input->w-1; i++)
	for (int j = 1; j < input->h-1; j++)
	{
        sumWhite =0;
		for (int k =-1; k <= 1; k++)
		for (int l =-1; l <= 1; l++)
		{
            if (inpPixels[i + k + (j + l) * input->w])
                sumWhite++;
		}

		Uint32* outpxl = (Uint32*)outPixels + j*output->w + i;

		*outpxl = ( sumWhite< 4) ?
            SDL_MapRGB(output->format,   0,   0,   0) :
            SDL_MapRGB(output->format, 255, 255, 255);
	}

	SDL_UnlockSurface(output);
	SDL_FreeSurface(input);
	return output;
}


int main(int argc, char** argv)
{
    if (argc == 1)
        ErrorMessage();

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
	printf("Attempting to wrap image from %s\n",path_in);
	SDL_Surface* wrapped = Wrapping_Copy(path_in, quad);
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
                asprintf(&str, "%s/thresholded_%i.png", path_out, i);
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
        IMGC_surface_to_grayscale(s);
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
        IMGC_to_grayscale(path_in, path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-agd") ||
                CompareStrings(argv[1], "--all>grid_detect"))
    {
        if (argc ==2)
        {
            for(int i =1 ; i<7;i++){
                if (i == 4 ) continue;
                asprintf(&path_in,"../test_grid/sudoku0%i.png",i);
                printf("Attempting to apply all from %s\n", path_in);
                SDL_Surface* s = IMG_Load(path_in);
                IMGC_surface_to_grayscale(s);
                s = sobel_gradient(IMGA_Erosion(CheckInvert(
                        IMGA_ApplyThreshold(
                            IMGA_GaussianBlur(s,Blursize, BlurIntensity),
                                            AdaptiveThreshold,Splitsize))));
                asprintf(&path_out,"./sudoku0%i.png",i);
                Quadrilateral* grid = Find_Grid(s);
                printQuad(grid);
                IMG_SavePNG(s, path_out);
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
        IMGC_surface_to_grayscale(s);

        s = sobel_gradient(IMGA_Erosion(CheckInvert(
                IMGA_ApplyThreshold(
                        IMGA_GaussianBlur( s,Blursize,BlurIntensity),
                                AdaptiveThreshold,Splitsize))));

        Quadrilateral* grid = Find_Grid(s);
        printQuad(grid);
        IMG_SavePNG(s, path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else
        ErrorMessage();

    return EXIT_SUCCESS;
}
