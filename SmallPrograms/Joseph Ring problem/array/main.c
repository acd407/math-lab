#include <stdio.h>
#include <stdlib.h>
int total,*people,step,p=0;
int length()
{
    int ret=0;
    for(int i=0;i<total;i++)
        if(people[i]==0)
            ret++;
    return ret;
}
void go(int s)
{
    int ss=s;//还需要走的步数
    while(ss>0)
    {
        if(p>=total)
            p-=total;
        if(people[p]==0)
        {
            p++;
            ss--;
        }
        else if(people[p]==1)
        {
            p++;
        }
    }
    while(people[p]==1)
    {
        p++;
        if(p>=total)
        p-=total;
    }
    people[p]=1;
    for(int i=0;i<total;i++)
    {    
        printf("\e[%dm%d ",32-people[i],people[i]);
    }
    printf("\e[33m\t%d\n", length());
}
int main(int argc,char **argv)
{
    total=atoi(argv[1]);
    people=calloc(total,sizeof(int));
    step=atoi(argv[2]);
    while(length()>1)
        go(step);
    return 0;
}