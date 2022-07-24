#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
using namespace std;
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

    SDL_Surface * flamesurface = IMG_Load("flame.png");
    SDL_Texture * flame = SDL_CreateTextureFromSurface(renderer,flamesurface);

    SDL_Surface * blacksurface = IMG_Load("black.png");
    SDL_Texture * black = SDL_CreateTextureFromSurface(renderer,blacksurface);
    SDL_SetTextureAlphaMod(black,64);

    SDL_Rect rect{0,0,flamesurface->w*16,flamesurface->h/2};
    bool right = true;
    bool down = true;

    SDL_Rect clips[32];
    for(int i=0;i<32;i++)
    {
        clips[i].x = 0;
        clips[i].y = flamesurface->h*i/32;
        clips[i].w = flamesurface->w;
        clips[i].h = flamesurface->h/32;
    }
    int clipsi = 0;
    SDL_Event events;
    bool loop = true;
    while(loop)//主循环
    {
        while (SDL_PollEvent(&events))
        {
            //quit
            if(events.type == SDL_QUIT)
            {
                loop = false;
            }
        }
        if(right)
            rect.x+=10;
        else
            rect.x-=10;
        if(down)
            rect.y+=10;
        else
            rect.y-=10;
        if(rect.x+flamesurface->w*16>WINDOW_WIDTH||rect.x<0)
            right = !right;
        if(rect.y+flamesurface->h/2>WINDOW_HEIGHT||rect.y<-30)
            down = !down;
        //SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,black,NULL,NULL);
        SDL_RenderCopy(renderer,flame,&clips[clipsi],&rect);
        if(++clipsi>=32)
            clipsi = 0;
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
    SDL_FreeSurface(flamesurface);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}