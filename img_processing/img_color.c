// IMGC FILE
// === IMAGE COLOR LIBRARY


#include "img_color.h"
#include "img_utils.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <stdio.h>



int main(int argc, char** argv)
{
    if (argc == 4)
    {
        if (strcmp(argv[1], "-g"))
            errx(EXIT_FAILURE, "Usage: -g image-file-in image-file-out");

        IMGC_to_grayscale(argv[2], argv[3]);
    }
    else if (argc == 5)
    {
        if (strcmp(argv[1], "-b"))
            errx(EXIT_FAILURE, "Usage: -b image-file-in image-file-out threshold");

        int threshold = atoi(argv[4]);
        IMGC_to_binary(argv[2], argv[3], threshold); 
    }
    else
        print_help();

    return EXIT_SUCCESS;
}

void  print_help()
{

    char* usage = "Usage: <image-in> <image-out> [threshold]\n"
        "       -b     Convert the given image in binary. Needs threshold\n"
        "       -g     Convert the given image in grayscale\n";
    printf("%s", usage);
}

/**
  * Convert the given <pixel_color> into a gray one using the average technique
  * in the given format.
  * Returns: The gray color
  */
Uint32 IMGC_pixel_to_grayscale(Uint32 pixel_color, const SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 avr = 0.3 * r + 0.59 * g + 0.11 * b;

    Uint32 gray = SDL_MapRGB(format, avr, avr, avr);
    return gray;
}


/**
  * Convert the given surface into grayscale
  * Surface is locked then pixels are modified one by one
  * === IN PLACE ===
  */
void IMGC_surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    
    SDL_LockSurface(surface);

    for (int i = 0; i < len; ++i)
    {
        pixels[i] = IMGC_pixel_to_grayscale(pixels[i], format);
    }

    SDL_UnlockSurface(surface);
}


Uint32 IMGC_pixel_to_binary(Uint32 pixel_color, const SDL_PixelFormat* format,
        int threshold)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    if (r < threshold)
        return SDL_MapRGB(format, 0, 0, 0); // returns White
    else
        return SDL_MapRGB(format, 255, 255, 255); // returns Black
}

void IMGC_surface_to_binary(SDL_Surface* surface, int threshold)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (int i = 0; i < len; ++i)
    {
        pixels[i] = IMGC_pixel_to_binary(pixels[i], format, threshold);                        
    }

    SDL_UnlockSurface(surface);
}

void IMGC_to_binary(const char* input_path, const char* output_path, int
        threshold)
{
    SDL_Surface* surface = IMGU_Load(input_path);

    IMGC_surface_to_grayscale(surface);
    IMGC_surface_to_binary(surface, threshold);

    SDL_SaveBMP(surface, output_path);

    SDL_FreeSurface(surface);


}

/** 
  * From the given input image file, export it to its grayscale version
  */
void IMGC_to_grayscale(const char* input_path, const char* output_path)
{
    SDL_Surface* surface = IMGU_Load(input_path);
    IMGC_surface_to_grayscale(surface);

    SDL_SaveBMP(surface, output_path);

    SDL_FreeSurface(surface);

}


