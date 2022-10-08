#include <stdio.h>
int main()
{
	for(int i=30;i<38;i++)
		printf("\e[%dm%d\e[0m\t\e[%dm%d\e[0m\n", i, i, i+10, i+10);
	return 0;
}