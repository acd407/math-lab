//此文件用于检测real方法能否在 z = z^2 + c 迭代中保持共轭

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double re;
    double im;
}COMPLEX;

void function(COMPLEX *z, COMPLEX*c) {
    double z_re_bak = z->re;
    z->re = z->re*z->re - z->im*z->im + c->re;
    z->im = 2*z_re_bak*z->im + c->im;
}

int main()
{
    COMPLEX z_1 = {0, 0};
    COMPLEX c_1 = {0.25, 0.25};
    COMPLEX z_2 = {0, 0};
    COMPLEX c_2 = {0.25, -0.25};
    for(int i=0;i<10000000;i++) {
        function(&z_1, &c_1);
        function(&z_2, &c_2);
        if (z_1.re!=z_2.re||z_1.im!=-z_2.im) {
            printf("z_1 %f + %f I\t", z_1.re, z_1.im);
            printf("z_2 %f + %f I\n", z_2.re, z_2.im);
        }
    }
    return 0;
}