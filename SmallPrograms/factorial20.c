#include<stdio.h>
int main()
{
	long long _=_^_;
	long long a=!_;
	long long b=a<<a;
	long long c=b^a;
	long long d=b<<a;
	long long e=d^a;
	long long f=e^c;
	long long g=f|a;
	long long h=b<<b;
	long long i=g&-g^h;
	long long j=a+b+c+d;
	long long k=i^b;
	long long l=c<<b;
	long long m=~g^~j;
	long long n=h|_|d|_|b;
	long long o=c|(c>>_<<b);
	long long p=~o&o+a;
	long long q=!!p|p;
	long long r=q+q-q*q/q;
	long long s=++r+a;
	long long t=s^g;
	printf("%lld",a*b*c*d*e*f*g*h*i*j*k*l*m*n*o*p*q*r*s*t);
}

