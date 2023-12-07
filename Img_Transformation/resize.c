#include "Img_Transformation/resize.h"

SDL_Surface* Resize(SDL_Surface* surface, int newSize) {
	SDL_Surface* resize = SDL_CreateRGBSurface(0,newSize,newSize,32,0,0,0,0);
	double scale = surface->w / (double)newSize;
	Uint32* pixels = resize->pixels;
	Uint32* p = surface->pixels;
	for (int x =0; x<newSize; x++) {
		for (int y = 0; y<newSize; y++) {
			pixels[x+y*newSize] = p[(int)(x*scale+(y*scale)*surface->w)];
		}
	}
	return resize;
}

Uint32 downscale_resize_average_box(SDL_Surface *surface, SDL_Surface *
        rsurface, int x, int y,
        int x_end, int y_end)
{
    Uint8 red, green, blue;
    float r_avr = 0;
    float g_avr = 0;
    float b_avr = 0;
        Uint32 *pixels = surface->pixels;
    for (int i = y; i < y_end; ++i)
    {
        unsigned long red_sum = 0;
        unsigned long green_sum = 0;
        unsigned long blue_sum = 0;

        for (int j = x; j < x_end; ++j)
        {
            Uint8 r, g, b;
            int pos = i * surface->pitch + j * surface->format->BytesPerPixel;
            //printf("pos is %i (%i, %i, %i)\n", pos, surface->w, surface->h,
            //        surface->w * surface->h);
            SDL_GetRGB(*(Uint32 *)((Uint8 *)pixels + pos), surface->format,
                    &r, &g, &b);
            //printf("rgb: %i %i %i\n", r, g, b);
            red_sum += r;
            green_sum += g;
            blue_sum += b;
        }
        r_avr += (float)red_sum / (x_end - x);
        b_avr += (float)blue_sum / (x_end - x);
        g_avr += (float)green_sum / (x_end - x);
    }
    //printf("r: %f g: %f b: %f\n", (r_avr / (y_end - y)),
//(g_avr / (y_end - y)),
//(b_avr / (y_end - y)));
    red   = (Uint8)ceil((r_avr / (y_end - y)));
    blue  = (Uint8)ceil((b_avr / (y_end - y)));
    green = (Uint8)ceil((g_avr / (y_end - y)));
    //printf("r: %i g: %i b: %i\n", red, green, blue);
    
    return SDL_MapRGBA(rsurface->format, red, green, blue, 255);
}

// cause the image to lose color?
//Uint32 downscale_resize_average_box_full(SDL_Surface *surface, int x, int y,
//        int x_end, int y_end)
//{
//    Uint32 *pixels = surface->pixels;
//    unsigned long long avr_sum = 0;
//    for (int i = y; i < y_end; ++i)
//    {
//        unsigned long long avr = 0;
//
//        for (int j = x; j < x_end; ++j)
//        {
//            avr += pixels[i * surface->w + j];
//        }
//         avr_sum += (float)avr / (x_end - x);
//    }
//    avr_sum = (avr_sum / (y_end - y));
//    return avr_sum;
//}
//
SDL_Surface *downscale_resize(SDL_Surface *surface, int new_width, int
        new_height)
{
    SDL_Surface *r_surface = SDL_CreateRGBSurface(0, new_width, new_height,
            32, 0,0,0,0);

    double scale_x = (float)new_width / surface->w;
    double scale_y = (float)new_height / surface->h;
    SDL_LockSurface(surface);
    SDL_LockSurface(r_surface);

    Uint32 *r_pixels = r_surface->pixels;

    int box_width = (int)ceil(1 / scale_x);
    int box_height = (int)ceil(1 / scale_y);
    for (int i = 0; i < new_height; ++i)
        for (int j = 0; j < new_width; ++j)
        {
            int x = (int)floor((j / scale_x));
            int y = (int)floor((i / scale_y));
            int x_end = fmin(x + box_width, surface->w - 1);
            int y_end = fmin(y + box_height, surface->h - 1);
            //printf("x: %i y: %i x_end: %i y_end: %i\n", j, i, x_end, y_end);
            Uint32 pixel = downscale_resize_average_box(
                    surface, r_surface, x, y, x_end, y_end);
            //printf("pixel: %i\n", pixel);
            r_pixels[i * new_width + j] = pixel;
        }

    SDL_UnlockSurface(r_surface);
    SDL_UnlockSurface(surface);
    return r_surface;
}

SDL_Surface* Resample(SDL_Surface* surface, int newSize) {
	SDL_Surface* resample = SDL_CreateRGBSurface(0,newSize,newSize,32,0,0,0,0);
	double scale = (double)newSize/(double)surface->w;
	Uint32* pixels = resample->pixels;
	Uint32* p = surface->pixels;
	for (int cy = 0; cy<newSize;cy++) {
		for(int cx = 0; cx<newSize;cx++){
			int pixel = (cy*(newSize)) + (cx);
			int nearestMatch = (((int)(cy/scale) * (surface->w)) + ((int)(cx/scale)));
			//printf("pixel: %i\nnearestMatch: %i\n",pixel,nearestMatch);
			pixels[pixel] = p[nearestMatch];
		}
	}
	return resample;
}

int toInt(char* c) {
	int total = 0;
	for(int i = 0; c[i];i++) {
		total*=10;
		total += c[i]-'0';
	}
	return total;
}

//// go into another file >:(
//int main(int argc, char** argv) {
//	if (argc != 5)
//		errx(1,"use : ./resize [input path] [output path] [size to resize to]\n");
//	SDL_Surface* resize = IMG_Load(argv[1]);
//	//IMG_SavePNG(Resample(resize,toInt(argv[3])),argv[2]);
//        SDL_Surface *r = downscale_resize(resize, toInt(argv[3]), toInt(argv[4]));
//        IMG_SavePNG(r, argv[2]);
//        SDL_FreeSurface(r);
//	SDL_FreeSurface(resize);
//	return 0;
//}
