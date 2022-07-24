#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#include <stdio.h>
int myAtoi(char *sz)
{
    int p = 0, sn = 0;

    while (sz[p] != 0)
    {
        if (sz[p] == '-')  
            sn = 1;
        else if (sz[p] >= '0' && sz[p] <= '9')
            break;
        p++;
    }
    int sum = 0;
    while (sz[p] >= '0' && sz[p] <= '9')
    {
        if(sum > INT_MAX/10.0)
            return sn == 1 ? -2147483648 : 2147483647;
        sum *= 10;
        sum += sz[p] - '0';
        p++;
    }
    return sn == 1 ? -sum : sum;
}

int main(int argc, char **argv)
{
    char *pp = "adr65";
    printf("%d", myAtoi(pp));
    return 0;
}
