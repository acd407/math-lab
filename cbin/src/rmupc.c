#include <stdio.h>
#include <ctype.h>

int main(int argc, char **argv) 
{ 
	FILE *fpi = NULL;
	FILE *fpo = NULL;
	char buffer = 0;
	if(argc < 3)
	{
		printf("ReMove UnPrintable Char\n");
		printf("Usage : %s SourceFilePath TargetFilePath\n", argv[0]);
		return 0;
	}
	fpi = fopen(argv[1], "r");
	if(fpi == NULL)
		puts("ERROR : Open Source File !\n");
	fpo = fopen(argv[2], "w+");
	
	while(1)
	{
		if((buffer = getc(fpi)) == EOF)
			break;
		if(isgraph(buffer))
		{
			putc(buffer, fpo);
		}
	}
	
	fclose(fpi);
	fclose(fpo);
	return 0; 
}

