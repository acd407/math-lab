#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
int main(int argc, char **argv)
{
	STARTUPINFO si = {0, 0};
	PROCESS_INFORMATION pi = {0};
	char *args = (char *)calloc(2048, sizeof(char));
	strcat(args, "D:\\MSYS2\\mingw64\\bin\\gcc.exe ");
    if(argc>1) {
        int i = 1;
        while (i<argc) {
            strcat(args, argv[i]);
            i++;
        }
    }

	TCHAR *szCmdLine = (TCHAR *)args;
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
	free(args);
	return 0;
}