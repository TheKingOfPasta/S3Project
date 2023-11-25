#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 IMGC_pixel_to_grayscale(Uint32 pixel_color,
    const SDL_PixelFormat* format);

SDL_Surface* IMGC_surface_to_grayscale(SDL_Surface* surface);
