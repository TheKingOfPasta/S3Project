#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "button.h"
#include <err.h>
#include <stdio.h>
#include "draw_interface.h"

#define DEFAULT_WIDTH 160
#define DEFAULT_HEIGHT 90//the default resolution of the screen,
						   //scale your gui items compared to these

void CallSDLError()
{
	errx(EXIT_FAILURE, "%s", SDL_GetError());
}

void Draw(SDL_Renderer* renderer, 
			button* list,
			size_t len,
			int width,
			int height)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	for (size_t i = 0; i < len; i++)
	{
		button b = list[i];
	
		int x = b.x * width / DEFAULT_WIDTH;
		int y = b.y * height / DEFAULT_HEIGHT;
		width = b.width * width / DEFAULT_WIDTH;
		height = b.height * height / DEFAULT_HEIGHT;


		SDL_RenderDrawLine(renderer, x, y,
								x + width, y);// top
		SDL_RenderDrawLine(renderer, x, y,
								x, y + height);// left
		SDL_RenderDrawLine(renderer, x + width, y,
								x + width, y + height);// right
		SDL_RenderDrawLine(renderer, x, y + height,
								x + width, y + height);// bottom
	}

	SDL_RenderPresent(renderer);
}

int main()
{
	SDL_Window* window =
		SDL_CreateWindow(
				"Sudoku OCR by Nord En Face",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				5*DEFAULT_WIDTH,
				5*DEFAULT_HEIGHT,
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

	int arraySize = 1;
	button buttonArray[arraySize];//Don't put buttons which go further than
		// 130 in x (that space is used to show the files you can choose from)
	buttonArray[0] = Button(50, 20, 60, 50, "Click me!", &DrawInterface);

	Draw(renderer, buttonArray, arraySize, width, height);

	while (1)
	{
		SDL_WaitEvent(&event);

		if (event.type == SDL_QUIT)
			break;
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			int x = event.button.x;
			int y = event.button.y;
			for (int i = 0; i < arraySize; i++)
			{
				button b = buttonArray[i];

				if (x >= b.x && x <= b.x + b.width &&
						y >= b.y && y <= b.y + b.height)
				{
					(b.func)(renderer, width, height);//Calls the function stored in the button
				}
			}
		}
		else if (event.type == SDL_WINDOWEVENT &&
				 event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
		
			SDL_GetWindowSize(window, &width, &height);
			Draw(renderer, buttonArray, arraySize, width, height);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
