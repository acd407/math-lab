#include <stdio.h>
int main(int argc, char **argv, char **env)
{
	int i = 0;
	while(env[i]!=0)
	{
		puts(env[i]);
		i++;
	}
	return 0;
}