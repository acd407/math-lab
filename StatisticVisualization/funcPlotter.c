#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define max(x, y) ((x)>(y)?(x):(y))

#define XDIM 1024    //此项必须为8的倍数，否则有BUG
#define YDIM 768

//为图形协调，请保持与XDIM、YDIM的比例
#define X_RANGE 2*4
#define Y_RANGE 2*3

#define INACCURACY 1.0 //单位：像素

#define XI XDIM/2
#define YI YDIM/2
bool tof(int i, int j)
{
    //坐标转换
    double x = (double)(i-XI)*X_RANGE/XDIM;
    double y = Y_RANGE-(double)(j+YI)*Y_RANGE/YDIM;
    double d = INACCURACY*max(X_RANGE/(double)XDIM, Y_RANGE/(double)YDIM);
    
    if
    ( 
        i == XI || j == YI-1 ||
        j == YI || i == XI-1 ||
        fabs(x-(int)x)<d    ||
        fabs(y-(int)y)<d    ||
        //添加函数
        fabs(y-x*x)<d       ||
        fabs(y-sin(x))<d    ||
        fabs(x+y*y*y-3*2*x*y+3)<2*d    ||
        fabs(y-exp(x))<d    ||
        fabs(y*y+x*x-2)<d   ||
        fabs(y-exp(-x*x))<d
    )
        return true;
    return false;
}

void writeBit(bool bit, FILE * fp)
{   //白色为true
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

int main() {
    FILE *fp;
    fp = fopen("picture.pbm","wb");
    fprintf(fp, "P4 %d %d\n", XDIM, YDIM);
    for(int j=0; j<YDIM; j++)
        for(int i=0; i<XDIM; i++)
            writeBit(tof(i, j), fp);
    fclose(fp);
    return 0;
}