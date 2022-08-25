#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XLEN 2560
#define YLEN 1600

int main()
{
    FILE *fp = fopen("out.ppm", "wb");
    
    fprintf(fp, "P6\n%d %d\n255\n", XLEN, YLEN);
    for(int i=0;i<YLEN;i++)
        for(int j=0;j<XLEN;j++)
            fprintf(fp, "%c%c%c", 
                i*256/YLEN, 
                j*256/XLEN, 
                255-((i-YLEN*j/XLEN)*(i-YLEN*j/XLEN)*256/XLEN/YLEN)
            );
    
    fclose(fp);
    return 0;
}