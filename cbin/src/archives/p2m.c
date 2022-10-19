#include <stdio.h>

int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("usage: %s str (needed to be converted).", argv[0]);
        return 0;
    }
    puts(argv[1]);
    return 0;
}