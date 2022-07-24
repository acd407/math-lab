#include <stdio.h>
#include <SDL2/SDL.h>
int main(int argc, char *argv[])
{
    SDL_Window *Window = NULL;
    SDL_Surface *Surface = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    Surface = SDL_GetWindowSurface(Window);
    SDL_FillRect(Surface, NULL, SDL_MapRGB(Surface->format, 0x80, 0x70, 0x60));
    SDL_UpdateWindowSurface(Window);
    while (1)
    {
        SDL_Event events;
        if (SDL_WaitEvent(&events))
        {
            if (events.type == SDL_QUIT)
                break;
            if (events.type == SDL_KEYDOWN)
            {
                if (events.key.keysym.sym == SDLK_ESCAPE)
                    break;
            }
        }
    }

    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
