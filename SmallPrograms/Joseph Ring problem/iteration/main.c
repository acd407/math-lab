#include <stdio.h>
/*计算约瑟夫环问题的迭代法函数*/
int Joseph(int n,int m)
{
    int i;
    int x,y;
    if(n <= 1 || m <= 1)
        return -1;
    if(m % 2 == 0)
        y = 1;
    else
        y = 2;

    for(i = 3; i <= n; i++)
    {
        x = (y-1 + m) % i + 1;
        y = x;
    }
    return y;
}

int main()
{
    int n,m,x;
    scanf("%d %d",&n,&m);
    x = Joseph(n,m);
    printf("最后一个的编号是：%d\n",x);
    return 0;
}