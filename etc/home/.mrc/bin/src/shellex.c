//gcc -s -Os shellex.c -lshell32 -o shellex.exe
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t ShellExecuteA(size_t, char *, char *, char *, char *, int);

int main (int argc, char **argv) {
    errno = 0;
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        exit(EXIT_FAILURE);
    if (argc == 1)
        ShellExecuteA(0,"open",cwd,"","",1);
    else
        for (int i=1;i<argc;i++) {
            ShellExecuteA(0,"open",argv[i],"","",1);
        }
    free(cwd);
    exit(EXIT_SUCCESS);
}
