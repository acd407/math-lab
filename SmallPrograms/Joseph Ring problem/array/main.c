#include <stdio.h>
#include <stdlib.h>
int length(int *people, int total)
{
    int ret=0;
    for(int i=0; i<total; i++)
        if(people[i]==0)
            ret++;
    return ret;
}
void go(int *people, int total, int step)
{
    static int p = 0;
    while(step>0) {
        if(p>=total)
            p-=total;
        if(people[p]==0) {
            p++;
            step--;
        } else if(people[p]==1)
            p++;
    }
    p--;
    people[p] = 1;
    for(int i=0; i<total; i++)
        printf("\e[%dm%d ",32-people[i],people[i]);
    printf("\e[33m\t%d\n", length(people, total));
}
int main(int argc,char **argv)
{
    if(argc!=3)
        return 0;
    int total=atoi(argv[1]);
    int *people=(int *)calloc(total,sizeof(int));
    int step=atoi(argv[2]);
    while(length(people, total)>1)
        go(people, total, step);
    int i;
    for(i=0;people[i];i++)
        printf("  ");
    printf("%d\n\e[0m", i+1);
    
    return 0;
}