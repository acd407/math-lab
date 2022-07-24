#include <stdio.h>
struct llv
{
	long long a;
	long long b;
};
struct llv fib(int n)
{
	struct llv ret = {1, 1};
	if(n==2||n==1)
		return ret;
	ret = fib(n-1);
	ret.b += ret.a;
	ret.a = ret.b - ret.a;
	return ret;
}
int main()
{
	for(int i=1;i<=90;i++)
		printf("%d\t%lld\n", i, fib(i).b);
}