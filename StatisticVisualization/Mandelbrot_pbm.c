#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <stdbool.h>

#define XDIM 1536    //此项必须为8的倍数，否则有BUG
#define YDIM 1536

#define NUM 1000    //迭代次数

// z = z*z + c

bool tof_complex(int i, int j)  //复数版本 (1536*1536->1000(1 core):15.128s)
{
    int n = NUM;
    //坐标转换 
    double complex c = (i-0.75*XDIM)/XDIM*2+(j-0.5*YDIM)*2/YDIM*I;
    double complex z = 0;
    while(n>0){
        z = z*z+c;
        if(cabs(z)>2)
            return false;
        n--;
    }
    return true;
}

struct COMPLEX
{
    double re;
    double im;
};

bool tof_real(int i,int j)  //纯实数版本 (1536*1536->1000(1 core):4.991s)
{
    int n = NUM;
    struct COMPLEX z = {0,0};
    struct COMPLEX c = {
        (i-0.75*XDIM)/XDIM*2,
        (j-0.5*YDIM)*2/YDIM
    };
    while(n>0){
        double z_re_bak = z.re;
        z.re = z.re*z.re-z.im*z.im+c.re;
        z.im = 2*z_re_bak*z.im+c.im;
        if(z.re>2||z.im>2)  //没必要用z.re*z.re+z.im*z.im>4，我们只关注是否收敛，不关心发散速度
            return false;
        n--;
    }
    return true;
}

void writeBit(bool bit, FILE * fp)
{
    static int bitNum = 0;
    static unsigned char Byte = 0;
    if(bitNum==8)
    {
        fputc(Byte, fp);
        Byte = 0;
        bitNum = 0;
    }
    Byte ^= bit<<(7-bitNum);
    bitNum++;
}

int main() 
{
    FILE *fp;
    fp = fopen("picture.pbm","wb");
    fprintf(fp, "P4 %d %d\n", XDIM, YDIM);
    for(int j=0; j<YDIM; j++)
        for(int i=0; i<XDIM; i++)
            writeBit(tof_complex(i, j), fp);
    fclose(fp);
    return 0;
}