#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>

int main(int argc, char **argv)
{
    STARTUPINFO si = {0, 0};
	PROCESS_INFORMATION pi = {0};
	char *str0 = "java -jar ";
	char *strt = (char *)malloc(1+strlen(argv[1])+strlen(str0));
	strcpy(strt, str0);
	strcat(strt, argv[1]);
	TCHAR szCmdLine[] = {TEXT(strt)};
	puts(strt);
    int retval;
	if(retval = 
		CreateProcess(
			NULL, 
			(LPSTR)szCmdLine, 
			NULL, 
			NULL, 
			FALSE, 
			0, 
			NULL, 
			NULL, 
			&si, 
			&pi
		)
	);
	else
	{
		printf( "CreateProcess failed (%d).\n", GetLastError() );
        return 1;
	}
}