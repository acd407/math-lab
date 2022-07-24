#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
double getnum(char *&str)
{
	double ret = 0;
	while(isdigit(*str))//整数部分
	{
		ret *= 10;
		ret += *str - 48;
		str++;
	}
	if(*str=='.')
	{
		str++;
		double rets = 0;
		while(isdigit(*str))//取到最后
			str++;
		char *retp = str;//保留字符串结尾位置位置
		str--;
		while(*str!='.')//小数部分
		{
			rets /= 10;
			rets += *str - 48;
			str--;
		}
		rets /= 10;
		ret += rets;
		str = retp;//恢复字符串结尾位置位置
	}
	return ret;
}
double node(char *&str)
{
	double ret = getnum(str);
	if(*str=='(')
		return ret+node(++str);
	if(*str=='+')
		return ret+node(++str);
	if(*str=='-')
		return ret-node(++str);
	if(*str=='*')
		return ret*node(++str);
	if(*str=='/')
		return ret/node(++str);
	if(*str==')')
	{
		str++;
		return ret;
	}
	if(*str==0)
		return ret;
	puts("error!");
	exit(-1);
}

void copystr(char *str)
{
	while(*++str)
		*(str-1) = *str;
	*(str-1) = 0;
}
void checknum(char *str)
{
	while(*str)
	{
		if(*str=='+'||*str=='-'||*str=='*'||*str=='/'||*str=='('||*str==')'||isdigit(*str))
		{
			str++;
			continue;
		}
		if(*str==' '||*str=='\t'||*str=='\n')
		{
			copystr(str);
			str++;
			continue;
		}
		puts("error: input string has illegal characters.");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	char str[] = "1*2*3+4";
	char *strp = str;
	checknum(str);
	printf("%llf\n", node(strp));
}