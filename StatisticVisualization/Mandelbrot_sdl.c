#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XDIM 1024
#define YDIM 1024

#define NUM 1024    //迭代次数

//虚拟坐标转换
#define i2c_re (i-0.75*XDIM)/XDIM*2*1 //XDIM/YDIM
#define j2c_im (j-0.5*YDIM)*2/YDIM  //0.5控制图形偏移

struct COMPLEX {
    double re;
    double im;
};
int tof(int i, int j)
{
    struct COMPLEX z = {0,0};
    struct COMPLEX c = {i2c_re, j2c_im};
    for(int t=1; t<=NUM; t++) {
        double z_re_bak = z.re;
        z.re = z.re*z.re-z.im*z.im+c.re;
        z.im = 2*z_re_bak*z.im+c.im;
        if(z.re>2||z.im>2)
            return t;
    }
    return 0;
}

struct RGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};
struct RGB translate2RGB(int code)
{
    struct RGB ret = {0, 0, 0}; //返回局部变量，危险
    if(code) {
        double log_code = log(code)/log(NUM);   //from 0 to 1
        ret.R = (int)(sqrt(log_code)*256);
        ret.G = (int)(log_code*256);
        ret.B = (int)((1-log_code)*(1-log_code)*256);
    }
    return ret;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *Window = SDL_CreateWindow("Draw Mandelbrot Set With SDL2",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          XDIM,YDIM,
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI
                                         );
    SDL_Renderer *renderer = SDL_CreateRenderer(Window,-1,SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    for(int j=0; j<YDIM; j++)
        for(int i=0; i<XDIM; i++) {
            struct RGB tof_ret = translate2RGB(tof(i, j));
            SDL_SetRenderDrawColor(renderer, tof_ret.R, tof_ret.G, tof_ret.B, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event events;
        if (SDL_WaitEvent(&events)) {
            if (events.type == SDL_QUIT)
                break;
            if (events.type == SDL_KEYDOWN) {
                if (events.key.keysym.sym == SDLK_ESCAPE)
                    break;
            }
        }
    }
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}