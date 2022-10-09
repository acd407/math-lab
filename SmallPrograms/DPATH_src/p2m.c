#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LOWER(x) ((x)<91?((x)+=32):(x))
int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("usage: %s str (needed to be converted).", argv[0]);
        return 0;
    }
    char *str = (char *)malloc(strlen(argv[1])+1);
    strcpy(str, argv[1]);
    for(int i=0;str[i];i++) {
        if(str[i]==':') {
            str[i]=LOWER(str[i-1]);
            str[i-1]='/';
        } else if(str[i]=='\\') {
            str[i]='/';
        } else if(str[i]==';') {
            str[i]=':';
        }
    }
    puts(str);
    free(str);
    return 0;
}