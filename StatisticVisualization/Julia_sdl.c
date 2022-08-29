#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sdl2/sdl.h>

#define PI 3.141592653589793

#define XDIM 256
#define YDIM 256

#define NUM 512    //迭代次数

#define RMNEG(_X_) ((_X_)>=0?(_X_):0)

struct {
    double x_offset;
    double y_offset;
    double x_range;
    double y_range;
} args;

struct COMPLEX
{
    double re;
    double im;
};

struct COMPLEX c;

int tof(int i, int j)
{
    struct COMPLEX z = {
        args.x_offset+(double)i/XDIM*args.x_range, 
        args.y_offset+(1-(double)j/YDIM)*args.y_range
    };
    for(int t=1;t<=NUM;t++){
        double z_re_bak = z.re;
        z.re = z.re*z.re-z.im*z.im+c.re;
        z.im = 2*z_re_bak*z.im+c.im;
        if(z.re*z.re+z.im*z.im>4)
            return t;
    }
    return 0;
}

struct RGB
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};
struct RGB translate2RGB(int code)
{
    struct RGB ret = {0, 0, 0}; //返回局部变量，危险
    if(code){
        double log_code = log(code)/log(NUM);   //from 0 to 1
        ret.R = (int)(RMNEG(cos(log_code*PI))*256);
        ret.G = (int)(RMNEG(sin(log_code*PI))*256);
        ret.B = (int)(RMNEG(-cos(log_code*PI))*256);
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 
        XDIM/2, 0, XDIM/2, YDIM
    );
    SDL_RenderDrawLine(renderer, 
        0, YDIM/2, XDIM, YDIM/2
    );
}

int main(int argc, char *argv[])
{
    if(argc==5){
        args.x_offset = atof(argv[1]);
        args.y_offset = atof(argv[2]);
        args.x_range  = atof(argv[3]);
        args.y_range  = atof(argv[4]);
    }else {
        args.x_offset = 0;
        args.y_offset = 0;
        args.x_range  = 1;
        args.y_range  = 1;
    }
    if(argc==3) {
        c.re = atof(argv[1]);
        c.im = atof(argv[2]);
    }else {
        srand(time(NULL));
        c.re = (double)rand()/RAND_MAX;
        c.im = (double)rand()/RAND_MAX;
    }
    printf("args.x_offset = %f\nargs.y_offset = %f\n", args.x_offset, args.y_offset);
    printf("args.x_range = %f\nargs.y_range = %f\n", args.x_range, args.y_range);
    printf("c = %f + %f I\n", c.re, c.im);
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
                        break;
                    case SDLK_DOWN:
                        args.y_offset-=args.y_range/5;
                        break;
                    case SDLK_LEFT:
                        args.x_offset-=args.x_range/5;
                        break;
                    case SDLK_RIGHT:
                        args.x_offset+=args.x_range/5;
                        break;
                    case SDLK_KP_MINUS:
                        args.x_offset-=0.5*args.x_range;
                        args.y_offset-=0.5*args.y_range;
                        args.x_range*=2;
                        args.y_range*=2;
                        break;
                    case SDLK_KP_PLUS:
                        args.x_offset+=0.25*args.x_range;
                        args.y_offset+=0.25*args.y_range;
                        args.x_range*=0.5;
                        args.y_range*=0.5;
                        break;
                    default:
                        //TODO
                        break;
                }
                draw(renderer);
                SDL_RenderPresent(renderer);
            }
        }
    }
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
