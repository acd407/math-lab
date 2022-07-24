#include <iostream>
#include <SDL2/SDL.h>
using namespace std;
int main(int  argc, char* argv[])
{
   
    SDL_Init(SDL_INIT_VIDEO);
    cout << "SDL initialized." << endl;

    SDL_Window *window = SDL_CreateWindow("SDL 4.6 2022.2.14",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,600,
                                          SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Surface *image = SDL_LoadBMP("guest.bmp");
    SDL_Rect rect{100,100};
    SDL_BlitSurface(image,NULL,surface,&rect);
    SDL_UpdateWindowSurface(window);

    SDL_Delay(3000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}