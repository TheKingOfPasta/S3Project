// IMGC FILE
// === IMAGE COLOR LIBRARY


#include "img_color.h"
#include "img_utils.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <err.h>



int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file-in image-file-out");

    IMGC_to_grayscale(argv[1], argv[2]);
    return 0;
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

