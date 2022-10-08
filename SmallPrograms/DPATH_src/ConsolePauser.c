#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("usage: %s ProgramName", argv[0]);
        exit(0);
    }
    int cmdLineLen = 1;
    for(int i=1;i<argc;i++)
		cmdLineLen += strlen(argv[i])+1;
    char *cmdLine = (char *)calloc(cmdLineLen, sizeof(char));
    for(int i=1;i<argc;i++)
    {
		strcat(cmdLine, argv[i]);
		strcat(cmdLine, " ");
	}
	
    struct timespec t_start = {0,0};
    struct timespec t_end = {0,0};
    STARTUPINFO si = {0, 0};
	PROCESS_INFORMATION pi = {0};
    
    BOOL working = CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if (!working)
	{
		DWORD error = GetLastError();
		printf("failed to create process, error code is %ld.\n", error);
		return 0;
	}
	
	clock_gettime(CLOCK_REALTIME,&t_start);	
	WaitForSingleObject(pi.hProcess, INFINITE);	
	clock_gettime(CLOCK_REALTIME,&t_end);
	
	unsigned long result;
	GetExitCodeProcess(pi.hProcess, &result);
	printf(
	    "return value: %ld\ntime: %.4lf ms.\n", 
        result, 
        (double)(t_end.tv_sec-t_start.tv_sec)*1000+(double)(t_end.tv_nsec-t_start.tv_nsec)/1000000
	);
}
