#include "gaussian_blur.h"

//void print_matrix(double** arr, size_t size)
//{
//	for (size_t j = 0; j < size; j++)
//	{
//		for (size_t i = 0; i < size; i++)
//		{
//			printf("%02f", arr[i][j]);
//		}
//		printf("\n");
//	}
//}

SDL_Surface* IMGA_GaussianBlur(SDL_Surface* surface, int size, double sigma)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);

	double gaussianWeights[size + 1][size + 1];

	for (int j = 0; j <= size; j++)
	{
		for (int i = 0; i <= size; i++)
		{
			double i2 = i - size / 2;
			double j2 = j - size / 2;
			gaussianWeights[i][j] =
				1000 * exp(-(i2*i2 + j2*j2) / (2 * sigma * sigma))
				/ (2 * M_PI * sigma * sigma);
		}
	}

	size /= 2;
    for (int i = 0; i < surface->w; i++)
    for (int j = 0; j < surface->h; j++)
    {
        Uint8 r;
        Uint8 g;
        Uint8 b;

        size_t total = 0;
        size_t totalWeight = 0;

        for (int k = -size; k <= size; k++)
        for (int l = -size; l <= size; l++)
        {
            if (i + k >= 0 &&
                i + k < surface->w &&
                j + l >= 0 &&
                j + l < surface->h)
            {
                SDL_GetRGB(
                    pixels[i + k + (j + l) * surface->w],
                    surface->format,
                    &r,
                    &g,
                    &b);

                size_t weight = gaussianWeights[k + size][l + size];

                totalWeight += weight;
                total += weight * r;//Since r = b = g (grayscale)

            }
        }

        newPixels[i + j * newS->w] =
            SDL_MapRGB(newS->format,
                        total / totalWeight,
                        total / totalWeight,
                        total / totalWeight);

    }

    SDL_FreeSurface(surface);

	return newS;
}
