//其实很容易证明Mandelbrot集是对称的
//注：记z的共轭为z*
//z[n+1] = z[n]^2 + c
//z'[n+1] = z'[n]^2 + c*
//z[1] = z'[1] = 0, 所以它们共轭
//若z[n]与z'[n]共轭，则z[n]^2与z'[n]^2共轭(易证)，所以若z[n+1]与z'[n+1]共轭
//数学归纳法，Mandelbrot集每一点关于实轴对称

#include <stdio.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdbool.h>

//z = z*z + c
bool charge(double c_re, double c_im)//from -1.5 to 1.5
{
    double complex c_1 = c_re+c_im*I;
    double complex c_2 = c_re-c_im*I;
    bool c_1_is_convergent = true;
    bool c_2_is_convergent = true;
    double complex z_c_1 = 0;
    double complex z_c_2 = 0;
    for(int i=0;i<500;i++) {
        z_c_1 = z_c_1*z_c_1+c_1;
        z_c_2 = z_c_2*z_c_2+c_2;
        if((cabs(z_c_1)>2)!=(cabs(z_c_2)>2))
            return false;
    }
    if(cabs(z_c_1)<2) {
        printf("%f : %f I\t", creal(z_c_1), cimag(z_c_1));
        printf("%f : %f I\n", creal(z_c_2), cimag(z_c_2));
    }
    return true;
}

int main()
{
    for(double re=-1.5;re<1.5;re+=0.1)
        for(double im=-1.5;im<1.5;im+=0.1){
            if(!charge(re, im)) 
                printf("%f : %f I\n", re, im);
        }
    return 0;
}