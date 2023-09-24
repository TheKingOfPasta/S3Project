#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "img_utils.h"

int main(int argc, char** argv)
{
    if (argc != 4)
        errx(EXIT_FAILURE, "Usage: image_in path_out angle");
    char* path_in = argv[1];
    char* path_out = argv[2];
    char* endptr;
    double angle = strtod(argv[3], &endptr);
}

/**
  * Rotate the texture with the given angle (from the center)
  *
  */
void IMGA_Rotate(SDL_Renderer* renderer, SDL_Texture* texture, const double angle)
{
    int error = SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL,
            SDL_FLIP_NONE);
    if (error)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
}


void IMGA_Rotate_from(char* path, const double angle)
{
    // TODO
}
