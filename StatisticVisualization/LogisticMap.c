#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define X 2560  //尽量为8的倍数，若不是X/8需向上取整
#define Y 1600  //推荐X、Y比例为(4.0-2.4):1，这样网格为正方形


#define b_start 2.4
#define b_end 4.0
#define b_len (b_end-b_start)

#define PI 3.14159265358979323846
#define function(_b, _x) (_b*sin(_x*PI)/4)
//迭代方程  
//x = b*x*(1-x) (_b*_x*(1-_x))
//x = b/4*sin(x/pi) (_b*sin(_x*PI)/4)
int main(int argc, char **argv)
{
    double x;
    if(argc==2)
        x = atof(argv[1]);
    else
        x = 0.6;

    unsigned char * data = (unsigned char *)calloc(X*Y/8, 1);
    
    double b = b_start;
    for(int i=0;i<X;i++){   //保证x轴取点密度
        double temp = x;
        for(int k=0;k<4000;k++){    //先迭代至基本稳定
            temp = function(b, temp);
        }
        for(int k=0;k<1000;k++){    //真正画的部分
            temp = function(b, temp);
            int vx = (b-b_start)/b_len*X;   //visual(virtual) x
            int vy = temp*Y;                //visual(virtual) y
            data[(Y-1-vy)*(X/8)+vx/8] |= 1<<(7-vx%8);
        }
        b += b_len/X;
    }
    //画竖线
    for(double i=b_start;i<b_end;i+=0.1){
        int vx = (i-b_start)/b_len*X;   //visual(virtual) x
        for(int vy=0;vy<Y;vy++){
            data[(Y-1-vy)*(X/8)+vx/8] |= 1<<(7-vx%8);
        }
    }
    //画虚线
    //{
    //    double lines[4] = {3.0, 3.4, 3.54, 3.57};
    //    for(int i=0;i<4;i++){
    //        int vx = (lines[i]-b_start)/b_len*X;   //virsual(virtual) x
    //        for(int vy=0;vy<Y;vy+=2){
    //            data[(Y-1-vy)*(X/8)+vx/8] |= 1<<(7-vx%8);
    //        }
    //    }
    //}
    //画横线
    for(double i=0;i<1;i+=0.1){
        int vy = i*Y;                   //virsual(virtual) y  
        for(int vx=0;vx<X;vx++){
            data[(Y-1-vy)*(X/8)+vx/8] |= 1<<(7-vx%8);
        }
    }
    //图片输出
    FILE *fp;
    fp = fopen("0.25 sin(PI x).pbm","wb");
    fprintf(fp, "P4 %d %d\n", X, Y);
    for(int i=0; i<X*Y/8; i++)
        fputc(data[i], fp);
    fclose(fp);
    
    free(data);
    return 0;
}
