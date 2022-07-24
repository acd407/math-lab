#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
using namespace std;
void UpdatePosition(SDL_Rect &rect)
{
    static bool moveright = true;
    static bool movedown = true;
    if(moveright)
    {
        rect.x++;
        if(rect.x+rect.w>=WINDOW_WIDTH)
            moveright = false;
    }
    else
    {
        rect.x--;
        if(rect.x<=0)
            moveright = true;
    }
    if(movedown)
    {
        rect.y++;
        if(rect.y+rect.h>=WINDOW_HEIGHT)
            movedown = false;
    }
    else
    {
        rect.y--;
        if(rect.y<=0)
            movedown = true;
    }
}
int main(int  argc, char* argv[])
{
   
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL 4.6 2022.2.14",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH,WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    SDL_Surface * surface = SDL_GetWindowSurface(window); 
    SDL_FillRect(surface,NULL,0);
    SDL_RenderPresent(renderer);

    SDL_Surface * pysurface = IMG_Load("python.jpg");
    SDL_Texture * py = SDL_CreateTextureFromSurface(renderer,pysurface);

    SDL_Rect rect{0,0,pysurface->w,pysurface->h};
    SDL_Event events;
    while(true)//主循环
    {
        while (SDL_PollEvent(&events))
        {
            //quit
            if(events.type == SDL_QUIT)
            {
                SDL_FreeSurface(pysurface);
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }//mouse
            /* else if(events.type == SDL_MOUSEMOTION)
            {
                rect = {events.motion.x-(pysurface->w/2)+1,
                        events.motion.y-(pysurface->h/2)+1,
                        pysurface->w,pysurface->h};
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer,py,NULL,&rect);
                SDL_RenderPresent(renderer);
            } */
            
        }
        UpdatePosition(rect);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,py,NULL,&rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(2);
    }
}