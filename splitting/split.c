#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void Split(char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    Uint32* pixels = surface->pixels;

    SDL_LockSurface(surface);//Avoid random stuff while accessing the pixels

    int w9 = surface->w / 9;
    int h9 = surface->h / 9;

    int fileIndex = 0;

    for (int j = 0; j < surface->h; j += h9)
    {
        for (int i = 0; i < surface->w; i += w9)
        {
            SDL_Surface* newS = SDL_CreateRGBSurface(0, w9, h9, 32, 0, 0, 0, 0);
            Uint32* newPixels = newS->pixels;

            for (int x = 0; x < w9; x++)
            for (int y = 0; y < h9; y++)
            {
                newPixels[y * w9 + x] = pixels[(j + y) * surface->w + (i + x)];
            }

            char* str;
            asprintf(&str, "split_%02i.png", fileIndex);
            IMG_SavePNG(newS, str);
            fileIndex += 1;
        }
    }

    SDL_FreeSurface(surface);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    Split(argv[1]);

    return 0;
}