#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>

//窗口大小
#define XDIM 512
#define YDIM 512

#define NUM 512   //迭代次数

#define PRINTOFFSET printf("offset = (%f, %f)\n", args.x_offset + args.x_range/2, args.y_offset + args.y_range/2)
#define PRINTRANGE printf("range = %.6e * %.6e\n", args.x_range, args.y_range)

struct {
    double x_offset;
    double y_offset;
    double x_range;
    double y_range;
} args;

struct COMPLEX {
    double re;
    double im;
};
int tof(int i, int j)
{
    struct COMPLEX z = {0,0};
    struct COMPLEX c = {
        args.x_offset+(double)i/XDIM*args.x_range, 
        args.y_offset+(1-(double)j/YDIM)*args.y_range
    };
    for(int t=2; t<NUM; t++) {
        double z_re_bak = z.re;
        z.re = z.re*z.re-z.im*z.im+c.re;
        z.im = 2*z_re_bak*z.im+c.im;
        if(z.re*z.re+z.im*z.im>4)
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
    if(code){
        double log_code = log(code)/log(NUM);
        ret.R = (int)(sqrt(log_code)*256);
        ret.G = (int)(log_code*256);
        ret.B = (int)((1-log_code)*(1-log_code)*256);
    }
    return ret;
}

void draw(SDL_Renderer *renderer)
{
    for(int j=0; j<YDIM; j++)
        for(int i=0; i<XDIM; i++) {
            struct RGB tof_ret = translate2RGB(tof(i, j));
            SDL_SetRenderDrawColor(renderer, tof_ret.R, tof_ret.G, tof_ret.B, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, XDIM/2, 0, XDIM/2, YDIM);
    SDL_RenderDrawLine(renderer, 0, YDIM/2, XDIM, YDIM/2);
}

int main(int argc, char *argv[])
{
    if(argc!=5){
        args.x_offset = 0;
        args.y_offset = 0;
        args.x_range  = 1;
        args.y_range  = 1;
    } else {
        args.x_offset = atof(argv[1]);
        args.y_offset = atof(argv[2]);
        args.x_range  = atof(argv[3]);
        args.y_range  = atof(argv[4]);
    }
    
    printf("args.x_offset = %f\nargs.y_offset = %f\n", args.x_offset, args.y_offset);
    printf("args.x_range = %f\nargs.y_range = %f\n", args.x_range, args.y_range);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *Window = SDL_CreateWindow("Draw Mandelbrot Set With SDL2",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          XDIM,YDIM,
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI
                                         );
    SDL_Renderer *renderer = SDL_CreateRenderer(Window,-1,SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);
    draw(renderer);
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event events;
        if (SDL_WaitEvent(&events)) {
            if (events.type == SDL_QUIT)
                break;
            if (events.type == SDL_KEYDOWN) {
                if(events.key.keysym.sym==SDLK_ESCAPE)
                    break;
                switch (events.key.keysym.sym) {
                case SDLK_UP:
                    args.y_offset+=args.y_range/5;
                    PRINTOFFSET;
                    draw(renderer);
                    break;
                case SDLK_DOWN:
                    args.y_offset-=args.y_range/5;
                    PRINTOFFSET;
                    draw(renderer);
                    break;
                case SDLK_LEFT:
                    args.x_offset-=args.x_range/5;
                    PRINTOFFSET;
                    draw(renderer);
                    break;
                case SDLK_RIGHT:
                    args.x_offset+=args.x_range/5;
                    PRINTOFFSET;
                    draw(renderer);
                    break;
                case SDLK_KP_MINUS:
                    args.x_offset-=0.5*args.x_range;
                    args.y_offset-=0.5*args.y_range;
                    args.x_range*=2;
                    args.y_range*=2;
                    PRINTRANGE;
                    draw(renderer);
                    break;
                case SDLK_KP_PLUS:
                    args.x_offset+=0.25*args.x_range;
                    args.y_offset+=0.25*args.y_range;
                    args.x_range*=0.5;
                    args.y_range*=0.5;
                    PRINTRANGE;
                    draw(renderer);
                    break;
                default:
                    //TODO
                    break;
                }
                SDL_RenderPresent(renderer);
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}