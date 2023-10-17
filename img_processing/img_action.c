#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <math.h>
#include "img_action.h"
#include "gaussian_blur.h"
#include "adaptive_thresholding.h"
#include "rotate.h"
#include "canny_edge_detector.h"

//1 -> a==b
//0 -> a!=b
int CompareStrings(char* a, char* b)
{
    for (; *a; a++)
    {
        if (*b != *a)
            return 0;
        b += 1;
    }
    return 1;
}

void ErrorMessage()
{
    errx(EXIT_FAILURE, "Usage: -r/--rotate\n"
           "                   -ra/--rotateauto\n"
           "                   -b/--blur\n"
           "                   -t/--threshold\n"
           "                   -s/--sobel\n");
}

int main(int argc, char** argv)
{
    if (argc == 1)
        ErrorMessage();

    char* path_in = argv[2];
    char* path_out = argv[3];

    if (CompareStrings(argv[1], "-b") || CompareStrings(argv[1], "--blur"))
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
            sigma = 1.5;
        }
        else if (argc == 4)
        {
            size = 11;
            sigma = 1.5;
        }
        else
            errx(EXIT_FAILURE, "-b/--blur : gaussian blur (path_in path_out [size, sigma])\n");
        
        printf("Attempting to blur image from %s\n", path_in);
        IMG_SavePNG(IMGA_GaussianBlur(IMG_Load(path_in), size, sigma), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-r") || CompareStrings(argv[1], "--rotate"))
    {
        if (argc != 5)
            errx(EXIT_FAILURE, "-r/--rotate : rotate at path (path_in path_out angle)\n");
        
        char* endptr;
        double angle = strtod(argv[4], &endptr);

        printf("Attempting to rotate image from %s\n", path_in);
        IMG_SavePNG(IMGA_Rotate_from(path_in, angle), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-ra") || CompareStrings(argv[1], "--rotateauto"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-ra/--rotateauto : rotate automatically at path (path_in path_out)\n");

        printf("Attempting to automatically rotate image from %s\n", path_in);
        IMG_SavePNG(IMGA_Rotate_auto(path_in), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-t") || CompareStrings(argv[1], "--threshold"))
    {
        if (argc == 4)
        {
            IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), 0), path_out);
            return EXIT_SUCCESS;
        }

        if (argc != 5 && argc != 6)
            errx(EXIT_FAILURE, "-t/--threshold : apply adaptive thresholding (path_in folder_path_out [threshold] [m]) (m (optional) : apply thresholds from 0 to n)\n");


        char* endptr;
        double threshold = strtod(argv[4], &endptr);

		char* str;

        printf("Attempting to apply thresholding image from %s\n", path_in);
		if (argc == 5)
			IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), threshold), path_out);
		else
            for (int i = 0; i <= threshold; i++)
            {
                asprintf(&str, "%s/thresholded_%i.png", path_out, i);	
                IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(path_in), i), str);
            }
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-s") || CompareStrings(argv[1], "--sobel"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-s/--sobel : apply sobel edge detection (path_in folder_path_out )\n");

        IMG_SavePNG(sobel_gradient(IMG_Load(path_in)), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }
    else if (CompareStrings(argv[1], "-bts") || CompareStrings(argv[1], "--blur>threshold>sobel"))
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "-s/--sobel : apply blur>threshold>sobel (path_in folder_path_out)\n");

        IMG_SavePNG(sobel_gradient(IMGA_ApplyThreshold(IMGA_GaussianBlur(IMG_Load(path_in), 11, 1.5), 0)), path_out);
        printf("Successfully saved the new image at path %s\n", path_out);
    }


    else
        ErrorMessage();

    return EXIT_SUCCESS;
}
