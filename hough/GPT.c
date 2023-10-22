#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void MyDrawLine(SDL_Surface* s, int x, int y, int x2, int y2)
{
    double vx = x2 - x;
    double vy = y2 - y;
    double len = sqrt(vx * vx + vy * vy);
    if (!len)
        return;//drawing from same point to same point

    vx /= len;
    vy /= len;
    double posX = x;
    double posY = y;

    Uint32* pixels = s->pixels;

    while (len)
    {
        pixels[(int)(posX + posY * s->w)] = SDL_MapRGB(s->format, 255, 0, 0);
        posX += vx;
        posY += vy;

        len -= 1;
    }
}

void houghTransform(SDL_Surface *inputSurface) {
    int maxRho = (int)sqrt(inputSurface->w*inputSurface->w + inputSurface->h*inputSurface->h);
    int *accumulator = (int *)malloc(maxRho * sizeof(int));

    for (int i = 0; i < maxRho; ++i) {
        accumulator[i] = 0;
    }

    for (int y = 0; y < inputSurface->h; ++y) {
        for (int x = 0; x < inputSurface->w; ++x) {
            Uint8 pixel = ((Uint8*)inputSurface->pixels)[y * inputSurface->pitch + x];
            if (pixel > 128) {
                for (int theta = 0; theta < 360; ++theta) {
                    double radians = theta * M_PI / 180.0;
                    int rho = (int)(x * cos(radians) + y * sin(radians));
                    if (rho > 0 && rho < maxRho) {
                        accumulator[rho]++;
                    }
                }
            }
        }
    }

    int maxVotes = 0;
    for (int i = 0; i < maxRho; ++i) {
        if (accumulator[i] > maxVotes) {
            maxVotes = accumulator[i];
        }
    }
	int i = 0;
    for (int rho = 0; rho < maxRho; ++rho) {
        if (accumulator[rho] > 0.8 * maxVotes) {
            	printf("Accumulator[%d]: %d\n",rho,accumulator[rho]);
		for (int theta = 0; theta < 360; ++theta) {
                double radians = theta * M_PI / 180.0;
                int x0 = (int)(rho * cos(radians));
                int y0 = (int)(rho * sin(radians));
                int x1 = x0 + 1000 * (-sin(radians));
                int y1 = y0 + 1000 * (cos(radians));
                if (x1 >= 0 && x1 < inputSurface->w && y1 >= 0 && y1 < inputSurface->h) {
                    	i++;
			MyDrawLine(inputSurface, x0 + inputSurface->w / 2, y0 + inputSurface->h / 2, x1 + inputSurface->w / 2, y1 + inputSurface->h / 2);
                }
            }
        }
    }
	printf("%i\n",i);
    free(accumulator);
}


SDL_Surface* load_image(const char* path)
{
    SDL_Surface* s = IMG_Load(path);
    if (s == NULL)
	errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(s,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL)
	errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(s);
    return surface;
}

int main(int argc, char *argv[]) {
	if (argc != 2)
		errx(1,"Use : ./hough [name of image]\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Hough Transform", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *inputSurface = load_image(argv[1]);
    houghTransform(inputSurface);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, inputSurface);

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(inputSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

