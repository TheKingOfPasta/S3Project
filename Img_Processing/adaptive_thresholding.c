#include "Img_Processing/adaptive_thresholding.h"

Uint8 OtsuThreshold(SDL_Surface *surface) {
    int width = surface->w;
    int height = surface->h;

    Uint32* pixels = (Uint32*)(surface->pixels);

    double histogram[256] = {0};

    //Calculate histogram
    Uint8 gray;
    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++) {
        SDL_GetRGB(pixels[x+y*width], surface->format,
                    &gray, &gray, &gray);
        histogram[gray]++;
    }

    //Normalise ~~ make into probs
    int totalPixels = width * height;
    for (int i = 0; i < 256; i++) {
        histogram[i] = (double)histogram[i] / totalPixels;
    }

    //Calculate the variance for all possible thresholds
    double maxVariance = -1.0;
    Uint8 optimalThreshold = 0;

    double sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += histogram[i];
        double w1 = 1.0 - sum;
        double mean0 = 0.0, mean1 = 0.0;

        for (int j = 0; j <= i; j++) {
            mean0 += j * histogram[j] / sum;
        }

        for (int j = i + 1; j < 256; j++) {
            mean1 += j * histogram[j] / w1;
        }

        double variance = sum * w1 * pow((mean0 - mean1), 2);

        if (variance > maxVariance) {
            maxVariance = variance;
            optimalThreshold = i;
        }
    }

    return optimalThreshold;
}

SDL_Surface* IMGA_OtsuThreshold(SDL_Surface* surface){
   SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    Uint8 otsuThreshold = OtsuThreshold(surface);

    SDL_LockSurface(newS);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);
    Uint8 gray;

    for (int i = 0; i < surface->w; i ++)
    for (int j = 0; j < surface->h; j ++)
    {
        SDL_GetRGB( pixels[i + (j) * surface->w],
                    surface->format,
                    &gray, &gray, &gray);
        newPixels[i+j*surface->w] =
        gray< otsuThreshold ?
            SDL_MapRGB(surface->format, 255, 255, 255) :
            0;
    }

    SDL_UnlockSurface(newS);
    SDL_FreeSurface(surface);
    return newS;

}

SDL_Surface* IMGA_Sauvola(SDL_Surface *s,int n, double k)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, s->w, s->h, 32,0,0,0,0);

    Uint32* pixels = (Uint32*)(s->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);
    Uint8 gray;

    n = s->w/100;
    if (!(n % 2)) n++;
    int size  = (n)>>1;
    SDL_LockSurface(newS);

    double R = 128;
    for (int i = 0; i<s->w; i++)
    for (int j = 0; j<s->h; j++)
    {
        double sumMean = 0;
        double sumsquared=0;
        Uint8 max = 0;
        int nbPxl = 0;
        for (int k = i-size; k <= i+size; k++)
        for (int l = j-size; l <= j+size; l++)
        {
            if(k<0 || l<0 || k>=s->w || l>=s->h)
                continue;
            nbPxl++;
            SDL_GetRGB(pixels[k + (l) * s->w],
                    s->format,
                    &gray, &gray, &gray);
            sumMean += gray;
            sumsquared += gray*gray;
            if(max < gray) max = gray;
        }
        double mean = sumMean / nbPxl;
        //double stdDeviation = sumsquared - 2*mean*sumMean + mean*mean*size;
        double stdDeviation =( sumsquared/nbPxl - mean*mean)/nbPxl;
        mean  = (mean + max)/2;
        int threshold = mean * ( 1.0- k*(1 - stdDeviation/R ));

        SDL_GetRGB( pixels[i + (j) * s->w],
                s->format,
                &gray, &gray, &gray);
        newPixels[i+j*s->w] = gray > threshold?
            0:
            SDL_MapRGB(s->format, 255, 255, 255) ;
    }

    SDL_UnlockSurface(newS);
    SDL_FreeSurface(s);
    return newS;
}

SDL_Surface* IMGA_ApplyThreshold(SDL_Surface* surface, int threshold, int split)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32,0,0,0,0);

    int splitSize = surface->w*surface->h/split;

    SDL_LockSurface(newS);

    Uint32* pixels = (Uint32*)(surface->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);
    Uint8 r,g,b;

    for (int i = 0; i < surface->w; i ++)
    for (int j = 0; j < surface->h; j ++)
    {
        int count = 0;
        Uint32 sum = 0;
        for (int k = -splitSize/2; k <= splitSize/2; k++)
        for (int l = -splitSize/2; l <= splitSize/2; l++)
        {
            int dx = i+k;
            int dy = j+l;
            if (dx >=0 && dx<surface->w && dy>=0 && dy<surface->h)
            {
                SDL_GetRGB(
                    pixels[dx + (dy) * surface->w],
                    surface->format,
                    &r,
                    &g,
                    &b);
                sum += r;
                count++;
            }
        }

        int average = sum/count;

        SDL_GetRGB(
            pixels[i + (j) * surface->w],
            surface->format,
            &r,
            &g,
            &b);
        newPixels[i+j*surface->w] =
            r< threshold +average-3 ?
            SDL_MapRGB(surface->format, 255, 255, 255) :
            0;
    }

    SDL_UnlockSurface(newS);
    SDL_FreeSurface(surface);
    return newS;
}