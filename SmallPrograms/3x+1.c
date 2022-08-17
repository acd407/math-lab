#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc!=2)
    {
        puts("argv[1]: the initializing number");
        return 0;
    }
    unsigned long long i = atoll(argv[1]);
    while(i!=1)
    {
        if(i%2==0) {
            i /= 2;
            printf("\e[32m%lld\e[0m\n", i);
        } else {
            i = 3*i+1;
            printf("\e[31m%lld\e[0m\n", i);
        }
    }
    return 0;
}