#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
int main(int  argc, char* argv[])
{
   
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL 4.6 2022.2.14",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Surface *picture = IMG_Load("python.jpg");
    SDL_Rect rect{0,0};
    for(int i=0;i<8;i++)
    {
        SDL_BlitSurface(picture,NULL,surface,&rect);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(300);
        rect.x+=20;
        rect.y+=20;
    }
    SDL_FreeSurface(picture);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}