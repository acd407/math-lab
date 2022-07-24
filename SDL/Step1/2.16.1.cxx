#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
using namespace std;

class Flame
{
public:
    int originalheight, originalwidth;
    int vx, vy;
    SDL_Rect rect;
    SDL_Rect clips[32];
    int clipsi = 0;
    SDL_Surface * flamesurface;
    SDL_Texture * flame;
    SDL_Surface * blacksurface;
    SDL_Texture * black;
    SDL_Renderer * renderer;
    Flame(){}
    Flame(SDL_Renderer * publicrenderer)
    {
        renderer = publicrenderer;
        flamesurface = IMG_Load("flame.png");
        flame = SDL_CreateTextureFromSurface(renderer,flamesurface);
        blacksurface = IMG_Load("black.png");
        black = SDL_CreateTextureFromSurface(renderer,blacksurface);
        SDL_SetTextureBlendMode(black,SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(black,64);
        originalheight = flamesurface->h;
        originalwidth = flamesurface->w;
        for(int i=0;i<32;i++)
        {
            clips[i].x = 0;
            clips[i].y = originalheight*i/32;
            clips[i].w = originalwidth;
            clips[i].h = originalheight/32;
        }
        rect.x = WINDOW_WIDTH/2;
        rect.y = WINDOW_HEIGHT/2;
        rect.w = clips[clipsi].w*8;
        rect.h = clips[clipsi].h*8;
        vx = 3;
        vy = 5;
    }
    ~Flame()
    {
        SDL_FreeSurface(flamesurface);
        SDL_FreeSurface(blacksurface);
    }
    void move()
    {
        vy += 3;//重力
        vx-=(vx>0?1:-1);
        //vy-=(vy>0?1:-1);//摩擦力
        rect.x += vx;
        rect.y += vy;
        if(rect.x<0)
        {
            //vx = -vx;
            rect.x=WINDOW_WIDTH-rect.w;
        }
        else if(rect.x+rect.w>WINDOW_WIDTH)
        {
            //vx = -vx;
            rect.x=0;
        }
        if(rect.y<0||rect.y+rect.h>WINDOW_HEIGHT)
        {
            vy = -vy;
            rect.y=WINDOW_HEIGHT-rect.h;
        }
        if(vy == 0&&rect.y+rect.h==WINDOW_HEIGHT)
            rect.y = 0;
    }
    void show()
    {
        SDL_RenderCopy(renderer,black,NULL,NULL);
        SDL_RenderCopy(renderer,flame,&clips[clipsi],&rect);
        if(++clipsi>=32)
            clipsi = 0;
    }
};



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
    
    Flame flame(renderer);

    SDL_Event events;
    bool loop = true;
    while(loop)
    {
        while(SDL_PollEvent(&events))
        {
            if(events.type == SDL_QUIT)
                loop = false;
            if(events.type == SDL_KEYDOWN)
            {
                switch(events.key.keysym.sym)
                {
                    case SDLK_UP:
                        flame.vy-=3;
                        break;
                    case SDLK_DOWN:
                        flame.vy+=3;
                        break;
                    case SDLK_LEFT:
                        flame.vx-=3;
                        break;
                    case SDLK_RIGHT:
                        flame.vx+=3;
                        break;
                }
            }
        }
        flame.move();
        flame.show();
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}                                      