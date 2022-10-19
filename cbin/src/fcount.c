//Count 什么玩意！
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 4*1048576
int main(int argc,char **argv)
{
    if(argc!=2)
    {
        printf("Usage: %s filename", argv[0]);
        return 0;
    }
    HANDLE fp = CreateFile(
        (LPCSTR)argv[1],
        GENERIC_READ | GENERIC_WRITE,
        0,NULL,OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,NULL
    );
    if(fp == INVALID_HANDLE_VALUE)
    {
        puts("File doesn't exist!");
        exit(EXIT_FAILURE);
    }
    char *buffer = (char *)malloc(BUFFERSIZE);
    int realsize;
    unsigned long long total = 0;
    if(ReadFile(fp,buffer,BUFFERSIZE,&realsize,NULL))
    {
        for(int i=0;i<BUFFERSIZE;i++)
            total += buffer[i];
    }
    else
    {
        for(int i=0;i<realsize;i++)
            total += buffer[i];
    }
    printf("%ld\n",total);
    free(buffer);
    return 0;
}
