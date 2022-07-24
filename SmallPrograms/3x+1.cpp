#include <iostream>
using namespace std;
int i;//main value
int n=0;
int fun()
{
	if(i%2==0)
	{
		i/=2;
		cout<<"\e[32m"<<i<<"\e[0m"<<'\n';
	}
	else
	{
		i=3*i+1;
		cout<<"\e[31m"<<i<<"\e[0m"<<'\n';
	}
	return 0;
}
int main(int argc, char**argv)
{
	if(argc==1)
		cin>>i;
	else  if(argc==2)
		i=atoi(argv[1]);
	else
	{
		cerr<<"arguments error!";
		exit(-1);
	}
	while(i!=1)
	{
		fun();
		n++;
	}
	cerr<<"Total : "<<n<<endl;
	return 0;
}
