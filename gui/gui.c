#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "button.h"
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "draw_text.h"
#include <dirent.h>

#include "../img_processing/adaptive_thresholding.h"
#include "../img_processing/canny_edge_detector.h"
#include "../img_processing/gaussian_blur.h"
#include "../img_processing/rotate.h"

#define DEFAULT_WIDTH 160
#define DEFAULT_HEIGHT 90//the default resolution of the screen,
						 //scale your gui items compared to these
void DrawInterface(SDL_Renderer* renderer, int width, int height, int scrollY, char* path, int selecting);
void CallSDLError()
{
	errx(EXIT_FAILURE, "%s", SDL_GetError());
}

void Render(SDL_Renderer* renderer, SDL_Texture* tex)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Draw(
	SDL_Renderer* renderer,
	button* buttonArr,
	int arrSize,
	int width,
	int height,
	int scrollY,
	char* path,
	int selecting)
{
    // Clears the renderer (sets the background to black).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Sets the color for drawing operations to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < arrSize; i++)
	{
		button b = buttonArr[i];
		int x = b.x * width / DEFAULT_WIDTH;
		int y = b.y * height / DEFAULT_HEIGHT;
		int w = b.width * width / DEFAULT_WIDTH;
		int h = b.height * height / DEFAULT_HEIGHT;

		SDL_Color color = {255, 255, 255, 255};

		DrawText(renderer, b.text, x, y, h, color);

		SDL_RenderDrawLine(renderer, x, y, x + w, y);
		SDL_RenderDrawLine(renderer, x, y, x, y + h);
		SDL_RenderDrawLine(renderer, x + w, y + h, x + w, y);
		SDL_RenderDrawLine(renderer, x + w, y + h, x, y + h);
	}
    
	DrawInterface(renderer, width, height, scrollY, path, selecting);

    // Updates the display.
    SDL_RenderPresent(renderer);
}


void DrawInterface(SDL_Renderer* renderer, int width, int height, int scrollY, char* path, int selecting)
{
    // Skeleton found on google,
    // don't blame me if it's ugly (*cough cough* loop condition)
	DIR* d = opendir(path);

    struct dirent* dir;

    if (d)
    {
        for(int i = 0; (dir = readdir(d)) != NULL; i++)
        {
            SDL_Color color;
        
            if (dir->d_type == 4)//Folder
                color = (SDL_Color){0, 255, 0, 255};
            else//File
                color = (SDL_Color){255, 255, 255, 255};

            int x = 130 * width / DEFAULT_WIDTH;
            int y = 10 * (i + scrollY) * height / DEFAULT_HEIGHT;
            int w = 30 * width / DEFAULT_WIDTH;
            int h = 10 * height / DEFAULT_HEIGHT;
			if (i == selecting)
			{//Draw cool arrow
				SDL_RenderDrawLine(renderer, 120 * width / DEFAULT_WIDTH, y + h/2, x, y + h/2);
				SDL_RenderDrawLine(renderer, x, y + h/2, x - 3 * width / DEFAULT_WIDTH, y - 3 * height / DEFAULT_HEIGHT + h/2);
				SDL_RenderDrawLine(renderer, x, y + h/2, x - 3 * width / DEFAULT_WIDTH, y + 3 * height / DEFAULT_HEIGHT + h/2);
			}

            DrawText(renderer, dir->d_name, x, y, h, color);

            SDL_RenderDrawLine(renderer, x, y, x + w, y);
            SDL_RenderDrawLine(renderer, x, y, x, y + h);
            SDL_RenderDrawLine(renderer, x + w, y + h, x + w, y);
            SDL_RenderDrawLine(renderer, x + w, y + h, x, y + h);
        }

        closedir(d);
    }
}

char* my_cat(char *str1, const char *str2)
{
    const size_t size1 = strlen(str1);
    const size_t size2 = strlen(str2);
    const size_t size_ab = size1 + size2 + 1;

    str1 = realloc(str1, size_ab);

    memcpy(str1 + size1, str2, size2 + 1);

    return str1;
}

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Window* window =
		SDL_CreateWindow(
				"Sudoku OCR by Nord En Face",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				10*DEFAULT_WIDTH,
				10*DEFAULT_HEIGHT,
				SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (!window)
		CallSDLError();

	SDL_Renderer* renderer =
			SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		CallSDLError();

	SDL_Event event;
	int width;
	int height;

	SDL_GetWindowSize(window, &width, &height);

	int selecting = -1;
	char* selectingText = malloc(2);
	selectingText[0] = '.';
	selectingText[1] = '\0';

	int arraySize = 4;
	button buttonArray[arraySize];//Don't put buttons which go further than
		// 130 in x (that space is used to show the files you can choose from)
	buttonArray[0] = Button(0, 0, "Gaussian blur", &IMGA_GaussianBlur);
	buttonArray[1] = Button(0, 20, "Thresholding", &IMGA_ApplyThreshold);
	buttonArray[2] = Button(0, 40, "Canny edge detection", &sobel_gradient);
	buttonArray[3] = Button(0, 60, "Rotate 15°", &IMGA_Rotate);

	int scrollY = 0;
	char* path = malloc(4);
	path[0] = '.';
	path[1] = '.';
	path[2] = '/';
	path[3] = '\0';

	Draw(renderer, buttonArray, arraySize, width, height, scrollY, path, -1);

	while (1)
	{
		SDL_WaitEvent(&event);

		if (event.type == SDL_QUIT)
			break;
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			int x = event.button.x;
			int y = event.button.y;

			if (x >= 130 * width / DEFAULT_WIDTH)
			{
				DIR* d = opendir(path);

				struct dirent* dir;

				if (d)
				{
					for(size_t i = 0; (dir = readdir(d)) != NULL; i++)
					{
						if (y > (int)(10 * (i + scrollY) * height / DEFAULT_HEIGHT) && y <= (int)(10 * (i + scrollY) * height / DEFAULT_HEIGHT + 10 * height / DEFAULT_HEIGHT))
						{
							if (dir->d_type == 8)//File
							{
								selecting = i;
								selectingText = strcpy(selectingText, path);
								selectingText = my_cat(selectingText, "/");
								selectingText = my_cat(selectingText, dir->d_name);
							}
							else//Folder
							{
								path = my_cat(path, "/");
								path = my_cat(path, dir->d_name);
								scrollY = 0;
							}
							Draw(renderer, buttonArray, arraySize, width, height, scrollY, path, selecting);
							break;
						}
					}
				}

				closedir(d);
			}


			for (int i = 0; i < arraySize; i++)
			{
				button b = buttonArray[i];

				if (x >= b.x * width / DEFAULT_WIDTH && x <= (b.x + b.width) * width / DEFAULT_WIDTH &&
						y >= b.y * height / DEFAULT_HEIGHT && y <= (b.y + b.height) * height / DEFAULT_HEIGHT)
				{
					//Dumb i know but multi params are annoying
					if (b.func == &IMGA_GaussianBlur)
					{
						IMG_SavePNG(IMGA_GaussianBlur(IMG_Load(selectingText), 11, 1.5), "../output.png");
						printf("Saved into root/output.png\n");
					}
					else if (b.func == &IMGA_ApplyThreshold)
					{
						IMG_SavePNG(IMGA_ApplyThreshold(IMG_Load(selectingText), 0), "../output.png");
						printf("Saved into root/output.png\n");
					}
					else if (b.func == &sobel_gradient)
					{
						IMG_SavePNG(sobel_gradient(IMG_Load(selectingText)), "../output.png");
						printf("Saved into root/output.png\n");
					}
					else if (b.func == &IMGA_Rotate)
					{
						IMG_SavePNG(IMGA_Rotate(IMG_Load(selectingText), 15), "../output.png");
						printf("Saved into root/output.png\n");
					}
					//Calls the function stored in the button
				}
			}
		}
		else if (event.type == SDL_WINDOWEVENT &&
				 event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDL_GetWindowSize(window, &width, &height);
			Draw(renderer, buttonArray, arraySize, width, height, scrollY, path, selecting);
		}
		else if (event.type == SDL_MOUSEWHEEL)
		{
			scrollY += event.wheel.y;
			if (scrollY > 0)
				scrollY = 0;
			
			Draw(renderer, buttonArray, arraySize, width, height, scrollY, path, selecting);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}