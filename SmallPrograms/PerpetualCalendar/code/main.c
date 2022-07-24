#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int year2day(int y)
{
	if(y%100==0)
		return (y%400==0)?366:365;
	else
		return (y%4==0)?366:365;
}

int main(int argc, char **argv)
{
	if(argc!=3){puts("error : need Year & Month!");exit(-1);}
	int year = atoi(argv[1]);
	int month = atoi(argv[2]);
	int delta = year%400;
	int day = 0;
	while(delta>1)
	{
		day+=year2day(delta);
		delta--;
	}				//算出基准差年对应的天数，基准年：1，401，801，1201...
	int monthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(year2day(year)==366)monthday[1] = 29;
	month-=2;		//减一：与数组配合 减一：天数不加本月份
	while(month>=0)
	{
		day+=monthday[month];
		month--;
	}				//算出总天数
	month = atoi(argv[2]);//恢复month的值
	
	//进入绘图部分
	int start = day%7;
	printf("====================%d年 %d月====================", year, month);
	//保证上端对齐
	for(int i=3-floor(log10(year));i>0;i--)
		putc('=', stdout);	//由年份对齐
	puts((month>9)?" ":"=");//由月份对齐
	
	puts("MON\tTUE\tWED\tTHU\tFRI\tSTA\tSUN");
	int count = 0;	//打印次数计数
	while(start>0)
	{
		putc('\t', stdout);
		start--;
		count++;
	}				//打印空白填充初始天数
	for(int i=1;i<=monthday[month-1];i++)
	{
		if(count==7)//7天换行
		{
			count=0;
			puts(" ");
		}
		printf("%d\t", i);
		count++;
	}
	puts("\n===================================================");
	return 0;
}