#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void DrawText(SDL_Renderer* renderer, char* text, int x, int y, int h, SDL_Color color)
{
	TTF_Init();

	TTF_Font * font = TTF_OpenFont("../fonts/arial.ttf", 100);//100 = quality of text

	SDL_Surface* txtSurface = TTF_RenderUTF8_Solid(font, text, color);
	SDL_Texture* txtTexture =
		SDL_CreateTextureFromSurface(renderer, txtSurface);

	SDL_FreeSurface(txtSurface);
	txtSurface = NULL;

	TTF_CloseFont(font);
	TTF_Quit();

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 20 * strlen(text) * 16 / 9;
	rect.h = h;

	SDL_RenderCopy(renderer, txtTexture, 0, &rect);
}