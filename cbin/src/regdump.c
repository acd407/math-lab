#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

// ==================== Data Struct Declare ====================
enum VALTYPE {				// MicroSoft UNICODE = ASCII byte followed by UNICODE byte
	REG_NONE=0x0000,		
	REG_SZ=0x0001,			// UNICODE character string 
	REG_EXPAND_SZ=0x0002,		// Unicode string with "%var%" expanded
	REG_BINARY=0x0003,		// Raw-binary value
	REG_DWORD=0x0004,		// Joe Normal DWord: 2 Words, 4 Bytes, 8 Nibbles, 32 Bits
	REG_DWORD_LITTLE_ENDIAN=0x0004,	// Joe Normal DWord: 2 Words, 4 Bytes, 8 Nibbles, 32 Bits
	REG_DWORD_BIG_ENDIAN=0x0005,	
	REG_LINK=0x0006,		
	REG_MULTI_SZ=0x0007,		// Multiple UNICODE strings followed by NULL (0x0000)
	REG_RESOURCE_LIST=0x0008,
	REG_FULL_RESOURCE_DESCRIPTOR=0x0009,
	REG_RESOURCE_REQUIREMENTS_LIST=0x000a
};

typedef struct
{
	int	recsize;		//0x00 DWord:	Length of sk record
	char	rectype[2];		//0x04 Word:	ID="v=sk", 0x6B73
	short	skuk001;		//0x06 Word:	sk Unknown 1 -- "Unused"
	int	prevsk;			//0x08 DWord:	Global offset of previous sk record
	int	nextsk;			//0x0C DWord:	Global offset of next sk record
	int	usecount;		//0x10 DWord:	Count of usage
	int	sksize;			//0x14 DWord:	Length of sk payload in bytes
	// a sk record is followed by sksize of bytes of security and auditing payload
} SK; //Security Key

typedef struct
{
	int	recsize;		//0x00 DWord:	Length of vk record
	char	rectype[2];		//0x04 Word:	ID="vk", 0x6B76
	short	namesize;		//0x06 Word:	Length of name
	int	datasize;		//0x08 DWord:	Length of data
	int	mydata;			//0x0C DWord:	Global offset to data
	int	valtype;		//0x10 DWord:	Type of value
	short	flag;			//0x14 Word:	Flag bits (bit[0]=0 then vk is "default")
	short	vkuk001;		//0x16 Word:	vk Unknown 1 -- "Unused (data-trash)"
	// a vk record is followed by namesize of chars of the vk's name
} VK; //Value Key

typedef struct
{
	int	nkrec;			//0x00 DWord:	Global offset of nk record
	char	pseudohash[4];		//0x04 DWord:	First four characters of nk record name
} HASH;

typedef struct
{
	int	recsize;		//0x00 DWord:	Length of lf record
	char	rectype[2];		//0x04 Word:	ID="lf", 0x666C
	short	numkeys;		//0x06 Word:	Number of keys
	// a lf record is followed immediately by numkeys of HASH records
} LF; //List? Fields?

typedef struct
{
	int	recsize;		//0x00 Dword:	Length of nk record
	char	rectype[2];		//0x04 Word:	ID="nk", 0x6B6E
	short	nktype;			//0x04 Word:	Type: 0x20 (0x2C for root key)
	int	datetime1;		//0x04 QWord:	NT Date Format
	int	datetime2;		//0x04 QWord:	NT Date Format
	int	parentnk;		//0x0C DWord:	Global offset to parent
	int	nkuk001;		//0x10 DWord:	nk Unknown 1
	int	numchildren;		//0x14 DWord:	Number of sub keys
	int	nkuk002;		//0x18 DWord:	nk Unknown 2
	int	mylfrec;		//0x1C DWord:	Global offset to lf record
	int	nkuk003;		//0x20 DWord:	nk Unknown 3
	int	numvalues;		//0x24 DWord:	Number of values = 0x00000000 if none
	int	myvallist;		//0x28 DWord:	Global offset to value list = 0xFFFFFFFF if none
	int	skrec;			//0x2C DWord:	Global offset to sk record
	int	myclassname;		//0x30 DWord:	Global offset to class name
	char	nkuk004[16];		//0x34 xByte:	nk Unknown 4
	int	nkuk005;		//0x44 DWord:	nk Unknown 5 -- "Unused (data-trash)"
	short	namesize;		//0x48 Word:	Length of name
	short	classnamesize;		//0x4A Word:	Length class name
	// a nk record is followed by namesize of chars of the nk's name
} NK; //Named? Key

typedef struct
{
	char	sig[4];			//0x00 Word:	ID="regf"
	int	regfuk001;		//0x04 DWord:	regf Unknown 1 = regfuk002
	int	regfuk002;		//0x08 DWord:	regf Unknown 2 = regfuk001
	int	ntdate1;		//0x0C QWord:	NT Date Format Chunk 1
	int	ntdate2;		//0x0C QWord:	NT Date Format Chunk 2
	int	regfuk003;		//0x14 DWord:	regf Unknown 3 = 1
	int	regfuk004;		//0x18 DWord:	regf Unknown 4 = 3
	int	regfuk005;		//0x1C DWord:	regf Unknown 5 = 0
	int	regfuk006;		//0x20 DWord:	regf Unknown 6 = 1
	int	firstkey;		//0x24 DWord:	Global offset of root nk record
	int	hivesize;		//0x28 DWord:	Sum of all 4096 byte hbins
	int	regfuk007;		//0x2C DWord:	regf Unknown 7 = 1
	int	regfuk008[115];		//0x30 DWord:	regf Unknown 8 = last accessed?
	int	cksum;			//0x1FC DWord:	Checksum
	char	balast[3584];		//Pure Junk = 0x00...

} REGF; //Registry File

// ==================== Functions Declare ====================

void printregf(REGF *regfrec);
int climbtree(char* base, NK* thisnk, int depth, char* path);
int printvk(char* base, VK* thisvk);
void printsz(int length, char* sz);
void printbin(int length, char* bin);

int main(int argc, char* argv[])
{

	FILE *hive;
	int numread, hivesize;
	REGF myregf;
	char* hbinptr;

	hive = fopen(argv[1],"r");
	if(hive)
	{		      
		numread = fread(&myregf,sizeof(char),sizeof(REGF),hive);
		printf("REGEDIT4\n",numread);
		printf("\n[HKEY_USERS]\n");

		hbinptr = (char*)malloc(myregf.hivesize);
		if(hbinptr)
		{
			numread = fread(hbinptr,sizeof(char),myregf.hivesize,hive);
			hivesize = climbtree(hbinptr,(NK*)&(hbinptr[myregf.firstkey]),0,"HKEY_USERS");
			free(hbinptr);
		}
	}
	else
		printf("File open failed!\n");
	fclose(hive);
	return 0;
}

int climbtree(char* base, NK* thisnk, int depth, char* path)
{
	char*	szname;
	char*	newpath;
	int	i,ii, subtreesize=0;
	int*	valptr;
	LF*		lfrec = (LF*) &(base[thisnk->mylfrec]);
	HASH*	hasharray = (HASH*) lfrec + 1;

	szname = (char*) malloc (thisnk->namesize+1);
	strncpy(szname,(char*)(thisnk+1),thisnk->namesize+1);
	szname[thisnk->namesize]= 0;

	subtreesize += thisnk->namesize + thisnk->classnamesize + thisnk->recsize + sizeof(NK);
	newpath = (char*)malloc(strlen(path)+1+thisnk->namesize+1);
	strncpy(newpath,path,strlen(path));
	strncpy(newpath+strlen(path),"\\",1);
	strncpy(newpath+(strlen(path)+1),(char*)(thisnk+1),thisnk->namesize);
	newpath[strlen(path)+1+thisnk->namesize]=0;
	printf("\n[%s]\n",newpath);

	if(thisnk->numvalues > 0)
	{
		valptr=(int*) &(base[thisnk->myvallist]);
		for(i=1; i<thisnk->numvalues+1; i++)	//not sure why this is one up --
		{										//maybe B.D. error
			if(valptr[i]>0)
			{
				subtreesize += printvk(base+4, (VK*) &(base[valptr[i]]));
			}
		}
	}

	if((thisnk->numchildren > 0) )//&& (depth<2))
	{
		for(i=0; i<thisnk->numchildren; i++)
		{
			subtreesize += climbtree(base, (NK*)&(base[(hasharray+i)->nkrec]), depth+1, newpath);
		}
	}
	return(subtreesize);
}


int printvk(char* base, VK* thisvk)
{
	char*	szname;
	int		vksize=sizeof(VK)+thisvk->namesize;

	szname = (char*) malloc (thisvk->namesize+1);
	strncpy(szname,(char*)(thisvk+1),thisvk->namesize+1);
	szname[thisvk->namesize]= 0;
	if(thisvk->flag && 0x0000000000000001)
		printf("\"%s\"=",szname);
	else
		printf("@=",szname);


	switch(thisvk->valtype)
	{
		case REG_SZ: 
				if(thisvk->datasize>0)
					printsz(thisvk->datasize,base+(thisvk->mydata));
			        else
					printf("\"\"\n");
			break;
		case REG_EXPAND_SZ: 
				if(thisvk->datasize>0)
				{
					printf("hex(2):\\\n");
					printbin(thisvk->datasize,base+(thisvk->mydata));
				}
			        else
					printf("\"\"\n");
			break;
		case REG_BINARY:  
				if(thisvk->datasize>0)
				{
					printf("hex:\\\n");
					printbin(thisvk->datasize,base+(thisvk->mydata));
				}
			break;
		case REG_MULTI_SZ:  
				if(thisvk->datasize>0)
				{
					printf("hex(7):\\\n");
					printbin(thisvk->datasize,base+(thisvk->mydata));
				}
			        else
					printf("\"\"\n");
			break;
		case REG_DWORD: printf("dword:%08x\n",thisvk->mydata); break;
	}

	if(thisvk->datasize>0)
		vksize+=thisvk->datasize;
	return(vksize);
}

void printsz(int length, char* sz)
{
	int i;
	printf("\"");
	for(i=0; i<length-2; i+=2)
	{
		printf("%c",(char)sz[i]);
		if((char)sz[i]=='\\')
		  printf("\\");
		if((char)sz[i]=='"')
		  printf("\"");
	}
	printf("\"\n");
}


void printbin(int length, char* bin)
{
	int i,ln;
	ln=0;
	for(i=0; i<length; i++)
	{
		printf("%02X",(unsigned char)bin[i]);
		ln+=2;
		if(i>=length-1) {
		  printf("\n");
		  ln=0;
		}
		else {
		  printf(",");
		  ln++;
		}
		if(ln >= 80) {
		  printf("\\\n");
		  ln=0;
		}
	}
}

