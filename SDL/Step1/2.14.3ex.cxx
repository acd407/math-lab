#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
int main(int argc, char *argv[])
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL 4.6 2022.2.14",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Surface *launch[74];

    char str[]{"./launch/launch_000.jpg"};
    launch[0] = IMG_Load(str);
    for (int i = 1; i <= 73; i++)
    {
        str[18]++;
        if (str[18] > '9')
        {
            str[18] = '0';
            str[17]++;
        }
        launch[i] = IMG_Load(str);
    }

    SDL_Rect rect{0, 0};
    SDL_Event events;
    while (true) //主循环
    {
        while (SDL_PollEvent(&events))
        {
            // quit
            if (events.type == SDL_QUIT)
            {
                for (int i = 0; i <= 73; i++)
                    SDL_FreeSurface(launch[i]);
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }
        for (int i = 0; i <= 73; i++)
        {
            SDL_BlitSurface(launch[i], NULL, surface, &rect);
            SDL_UpdateWindowSurface(window);
            SDL_Delay(100);
        }
    }
}