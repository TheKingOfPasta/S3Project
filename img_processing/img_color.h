#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 IMGC_pixel_to_grayscale(Uint32 pixel_color, const SDL_PixelFormat* format);

void IMGC_surface_to_grayscale(SDL_Surface* surface);

void IMGC_to_grayscale(const char* input_path, const char* output_path);

void print_help();

void IMGC_to_binary(const char* input_path, const char* output_path, int
        threshold);

void IMGC_surface_to_binary(SDL_Surface*, int threshold);

