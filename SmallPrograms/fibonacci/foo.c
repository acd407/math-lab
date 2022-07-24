#include <stdio.h>
int main()
{
	unsigned long long a = 1, b = 1;
	for(int i=3;i<93;i++)
		printf("%d\t%lld\n", i, (b+=a, a=b-a, b));
	return 0;
}