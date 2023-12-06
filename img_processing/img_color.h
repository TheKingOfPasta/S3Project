#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 IMGC_pixel_to_grayscale(Uint32 pixel_color,
    const SDL_PixelFormat* format);

SDL_Surface* IMGC_Grayscale(SDL_Surface* surface);
SDL_Surface* IMGC_Gamma_Correction(SDL_Surface* surface, int gamma);
SDL_Surface* IMGC_Contrast_Correction(SDL_Surface* surface, int contrast);
SDL_Surface* IMGC_Normalize_Brigthness(SDL_Surface* surface);
SDL_Surface* IMGC_Level_Colors(SDL_Surface* surface, int n);