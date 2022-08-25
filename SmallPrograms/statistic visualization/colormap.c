#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XLEN 1024
#define YLEN 1024

int main()
{
    FILE *fp = fopen("out.ppm", "wb");
    
    fprintf(fp, "P6\n%d %d\n255\n", XLEN, YLEN);
    for(int i=0;i<XLEN;i++)
        for(int j=0;j<YLEN;j++)
            fprintf(fp, "%c%c%c\0", 
                i*256/XLEN, 
                j*256/YLEN, 
                255-((i-j)*(i-j)*256/XLEN/YLEN)
            );//RGB
    
    fclose(fp);
    return 0;
}