#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>

#define DEFAULT_WIDTH 160
#define DEFAULT_HEIGHT 90

void DrawText(SDL_Renderer* renderer,
                int x,
                int y,
                char* text,
                int width,
                int height)
{
    x *= width / DEFAULT_WIDTH;
    y *= height / DEFAULT_HEIGHT;//Convert to absolute positions

//Implement something like SDL_DrawText or whatever here when I learn how to
}

//Responsible for drawing all the files so you can choose one
void DrawInterface(SDL_Renderer* renderer, int width, int height)
{
    // Skeleton found on google,
    // don't blame me if it's ugly (*cough cough* loop condition)
	DIR *d = opendir("../");// ../ == S3Project root directory
    struct dirent* dir;

    if (d)
    {
        for(size_t i = 0; (dir = readdir(d)) != NULL; i++)
        {
            if (dir->d_name[0] != '.')
            {
                DrawText(renderer, 130, i + 10, dir->d_name, width, height);
                if (dir->d_type == 4)//Directory == 4 | file == 8
                    printf("\x1b[32m");
                else
                    printf("\x1b[0m");
                printf("%s\n", dir->d_name);
            }
        }

        closedir(d);
    }
}
