#include <stdio.h>

char szhex2hex(char szhex)
{
	if(szhex >= 0x30 && szhex <= 0x39)
		return szhex - 0x30;
	else if(szhex >= 0x41 && szhex <= 0x46)
		return szhex - 0x37; 
	puts("Source file error !");
	return -1;
}

int translate(char *buffer, FILE *fpo)
{
	char ret;
	ret = szhex2hex(buffer[0]) * 0x10 + szhex2hex(buffer[1]);
	if(fwrite(&ret, 1, 1, fpo))
		return 0;
	return 1;
}

int main(int argc, char **argv) 
{ 
	FILE *fpi = NULL;
	FILE *fpo = NULL;
	char buffer[2] = {0};
	if(argc == 1)
	{
		printf("Usage : %s SourceFilePath TargetFilePath", argv[0]);
		return 0;
	}
	fpi = fopen(argv[1], "rb");
	fpo = fopen(argv[2], "w+b");
	while(1)
	{
		if(fread(buffer, 1, 2, fpi) != 2)
			break;
		if(translate(buffer, fpo))
		{
			puts("Error !");
			return 1;
		}
	}
	
	fclose(fpi);
	fclose(fpo);
	return 0; 
}