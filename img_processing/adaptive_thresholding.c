#include "adaptive_thresholding.h"

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

SDL_Surface* IMGA_AdaptiveThresholdDeluxe(SDL_Surface* s, int split)
{
    SDL_Surface* newS =
        SDL_CreateRGBSurface(0, s->w, s->h, 32,0,0,0,0);

    int splitSize = s->w*s->h/split;
    int w = s->w, h=s->h;

    int otsuThreshold = OtsuThreshold(s);
    int halfSplit = splitSize>>1;

    SDL_LockSurface(newS);

    Uint32* pixels = (Uint32*)(s->pixels);
    Uint32* newPixels = (Uint32*)(newS->pixels);
    Uint8 gray;

    long sumImg[h*w];
    long sum = 0;
    for (int i = 0; i < w; i++)
    {
        SDL_GetRGB(pixels[i],
            s->format,
            &gray,&gray,&gray);
        sum += gray;
        sumImg[i] = sum;
    }
    for(int i = 0; i<w; i++){
        sum = 0;
        for(int j = 1; j<h; j++)
        {
            SDL_GetRGB(pixels[i+j*w],
                s->format,
                &gray,&gray,&gray);
            sum += gray;
            sumImg[i+j*w] = sumImg[i+(j-1)*w] + sum;
        }
    }

    int x1, y1, x2, y2;
    long count;
    for (int i = 0; i < w; i ++)
    for (int j = 0; j < h; j ++)
    {
        x1 =  i - halfSplit > 1 ? i - halfSplit : 1;
        y1 =  j - halfSplit > 1 ? j - halfSplit : 1;
        x2 =  i + halfSplit < w - 1 ? i + halfSplit : w - 1;
        y2 =  j + halfSplit < h - 1 ? j + halfSplit : h - 1;
        count = (x2 - x1) * (y2 - y1);
        x1--;y1--;
        sum = sumImg[x2 + w*y2]
            - sumImg[x2 + w*y1]
            - sumImg[x1 + w*y2]
            + sumImg[x1 + w*y1];
        double avg = ((double)(sum))/count;
        SDL_GetRGB(pixels[i],
            s->format,
            &gray,&gray,&gray);
        newPixels[i+j*s->w] = gray< avg ?
            SDL_MapRGB(s->format, 255, 255, 255) :
            0;
    }

    SDL_UnlockSurface(newS);
    SDL_FreeSurface(s);
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

    for (int i = size; i<s->w-size; i++)
    for (int j = size; j<s->h-size; j++)
    {
        double sumMean = 0;
        double sumsquared=0;
        Uint8 max = 0;
        for (int k = i-size; k <= i+size; k++)
        for (int l = j-size; l <= j+size; l++)
        {
            SDL_GetRGB(pixels[k + (l) * s->w],
                    s->format,
                    &gray, &gray, &gray);
            if(max < gray) max = gray;
            sumMean += gray;
            sumsquared += gray*gray;
        }
        double mean = sumMean / (n*n);
        //double stdDeviation = sumsquared - 2*mean*sumMean + mean*mean*n*n;
        double stdDeviation = sumsquared + mean* (-2*sumMean + mean*n*n);
        double range = 128;
        mean  = (mean + max)/2;
        int threshold = mean * ( 1.0- k*(1 - stdDeviation/range ));

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