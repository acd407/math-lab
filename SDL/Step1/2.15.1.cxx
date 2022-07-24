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
                                          1000,800,
                                          SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Surface * py = IMG_Load("python.jpg");

    SDL_Rect rect{0,0};
    SDL_Event events;
    while(true)//主循环
    {
        while (SDL_PollEvent(&events))
        {
            //quit
            if(events.type == SDL_QUIT)
            {
                SDL_FreeSurface(py);
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }//mouse
            else if(events.type == SDL_MOUSEMOTION)
            {
                rect = {events.motion.x-(py->w/2)+1,events.motion.y-(py->h/2)+1};
                SDL_FillRect(surface,NULL,0);
                SDL_BlitSurface(py,NULL,surface,&rect);
                SDL_UpdateWindowSurface(window);
            }
        }
        
    }
}