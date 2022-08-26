#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <stdbool.h>

#define XDIM 2560
#define YDIM 1600

#define NUM 1024    //迭代次数

// z = z*z + c

//虚拟坐标转换
#define i2c_re (i-0.7*XDIM)/XDIM*2*1.6
#define j2c_im (j-0.5*YDIM)*2/YDIM

bool tof_complex(int i, int j)
{
    double complex c = i2c_re+j2c_im*I;
    double complex z = 0;
    for(int t=1;t<=NUM;t++){
        z = z*z+c;
        if(cabs(z)>2)
            return t;
    }
    return 0;
}

struct COMPLEX
{
    double re;
    double im;
};

int tof_real(int i, int j)
{
    struct COMPLEX z = {0,0};
    struct COMPLEX c = {i2c_re, j2c_im};
    for(int t=1;t<=NUM;t++){
        double z_re_bak = z.re;
        z.re = z.re*z.re-z.im*z.im+c.re;
        z.im = 2*z_re_bak*z.im+c.im;
        if(z.re>2||z.im>2)
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
        ret.R = (int)(sqrt(log_code)*256);
        ret.G = (int)(log_code*256);
        ret.B = (int)((1-log_code)*(1-log_code)*256);
    }
    return ret;
}

int main()
{
    FILE *fp = fopen("out.ppm", "wb");
    fprintf(fp, "P6\n%d %d\n255\n", XDIM, YDIM);
    for(int j=0;j<YDIM;j++)
        for(int i=0;i<XDIM;i++)
        {
            struct RGB tof_ret = translate2RGB(tof_real(i, j));
            fprintf(fp, "%c%c%c", tof_ret.R, tof_ret.G, tof_ret.B);
        }
    
    fclose(fp);
    return 0;
}