#include "Img_Processing/img_color.h"

/**
  * Convert the given surface into grayscale
  */
SDL_Surface* IMGC_Grayscale(SDL_Surface* surface)
{
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;

  SDL_LockSurface(surface);
  Uint8 r, g, b;

  for (int i = 0; i < surface->w; ++i)
  for (int j = 0; j < surface->h; ++j)
  {
	int offset = j * surface->pitch + i * format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format, &r, &g, &b);
	Uint8 avr = 0.4 * r + 0.35 * g + 0.25 * b;
	pixels[i+surface->w*j] = SDL_MapRGB(format, avr, avr, avr);
  }
  SDL_UnlockSurface(surface);

  return surface;
}

SDL_Surface* IMGC_Gamma_Correction(SDL_Surface* surface, int gamma)
{
  Uint32* pixels = surface->pixels;

  SDL_PixelFormat* format = surface->format;

  SDL_LockSurface(surface);
  Uint8 gray;

  for (int i = 0; i < surface->w; ++i)
  for (int j = 0; j < surface->h; ++j)
  {
	int offset = j * surface->pitch + i * format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
	   &gray, &gray, &gray);

	double a = pow(gray / 255.0, gamma / 128.0) * 255.0;

	Uint8 newVal = a> 255 ?  255 : a;

	pixels[i+surface->w*j] = SDL_MapRGB(format, newVal, newVal, newVal);
  }

  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface* IMGC_Contrast_Correction(SDL_Surface* surface, int contrast)
{
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;

  SDL_LockSurface(surface);
  Uint8 gray;

  for (int i = 0; i < surface->w; ++i)
  for (int j = 0; j < surface->h; ++j)
  {
	int offset = j * surface->pitch + i * format->BytesPerPixel;
	SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
	  &gray, &gray, &gray);

	contrast = contrast> 128? 128 : contrast;
	double a = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
	double b = a*(gray - 128) + 128;
	Uint8 newVal =b < 0 ? 0 : (b>255? 255 : b);

	pixels[i+surface->w*j] = SDL_MapRGB(format, newVal, newVal, newVal);
  }

  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface* IMGC_Normalize_Brigthness(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;

	SDL_LockSurface(surface);

	Uint8 gray,max =0 , min = 255;
	for (int i = 2; i < surface->w-2; ++i)
	for (int j = 2; j < surface->h-2; ++j)
	{
		int offset = j * surface->pitch + i * format->BytesPerPixel;
		SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
		&gray, &gray, &gray);

		if(max<gray)max=gray;
		if(min>gray)min=gray;

	}

	Uint8 diff = max-min;
	for (int i = 0; i < surface->w; ++i)
	for (int j = 0; j < surface->h; ++j)
	{
		int offset = j * surface->pitch + i * format->BytesPerPixel;
		SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
		&gray, &gray, &gray);

		Uint8 newVal = (gray - min)*255/diff ;

		pixels[i+surface->w*j] = SDL_MapRGB(format, newVal, newVal, newVal);
	}

	SDL_UnlockSurface(surface);
	return surface;
}

SDL_Surface* IMGC_Level_Colors(SDL_Surface* surface, int n)
{
	Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;

	SDL_LockSurface(surface);

	Uint8 gray,newVal;
	float multi = (255.f/n);

	for (int i = 0; i < surface->w; ++i)
	for (int j = 0; j < surface->h; ++j)
	{
		int offset = j * surface->pitch + i * format->BytesPerPixel;
		SDL_GetRGB(( *(Uint32*)((Uint8*)pixels + offset)), format,
			&gray, &gray, &gray);

		newVal = round(gray/multi)*multi;
		pixels[i+surface->w*j] = SDL_MapRGB(format, newVal, newVal, newVal);
	}

  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface* CheckInvert(SDL_Surface* img)
{
	Uint32* pixels = (Uint32*)(img->pixels);
	int whiteCellCount = 0;
	int blackCellCount = 0;

	for (int i = 0; i < img->w; i++)
	for (int j = 0; j < img->h; j++)
	{
		if ((Uint8)(pixels[i + j * img->w]) == 0)
			blackCellCount += 1;
		else
			whiteCellCount += 1;
	}

	if (whiteCellCount > blackCellCount)//Inverse
	{
		SDL_LockSurface(img);

		for (int i = 0; i < img->w; i++)
		for (int j = 0; j < img->h; j++)
		{
			int red = (Uint8)(pixels[i + j * img->w]);
			pixels[i + j * img->w] = SDL_MapRGB(img->format,
												255 - red,
												255 - red,
												255 - red);
		}

		SDL_UnlockSurface(img);
	}

	return img;
}