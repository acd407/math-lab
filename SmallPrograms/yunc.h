#define C
//#define CPP
#ifdef C
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
#endif

#define abs(x) (x)>=0?(x):-(x)
#define max(x, y) ((x)>(y)?(x):(y))
#define min(x, y) ((x)<(y)?(x):(y))

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int isPrime(int n)
{	//在此没有进行输入异常检测
	int n_sqrt = floor(sqrt((float)n));
	if(n==2 || n==3) 
		return true;
	if(n%6!=1 && n%6!=5) 
		return false;
	for(int i=5;i<=n_sqrt;i+=6)
	    if(n%i==0||n%(i+2)==0) 
			return false;
    return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double ret = 1;
int subPow(double x, int n)
{
    if(n==1)
    {
        ret*=x;
        return 0;
    }
    else if(n%2==0)
            subPow(x*x,n/2);
    else
    {
        ret*=x;
        subPow(x*x,(n-1)/2);
    }
    return 0;
}
double myPow(double x, int n)
{
    if(n==0||x==1)
        return 1;
    return (subPow(x,abs(n)),n>0)?ret:1/ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int szexistQ(char *s,int l,char *sz)
{
    for(int j=0;sz[j]!=0;j++)
    {
        for(int i=0;i<l;i++)
        {
            if(s[i]==sz[j])
                goto success;
        }
        return 0;
        success: ;
    }
    return 1;
}

void dropsz(char *s,int l,char *sz)
{
    for(int j=0;sz[j]!=0;j++)
        for(int i=0;i<l;i++)
            if(s[i]==sz[j])
            {    
                s[i]=0;
                break;
            }
}

char numc[10] = {'z','w','u','o','r','g','f','v','x','n'};
char numsz[10][6] = {"zero","two","four","one","three","eight","five","seven","six","nine"};
int sz2n[10] = {0,2,4,1,3,8,5,7,6,9};
int count[10] = {0,0,0,0,0,0,0,0,0,0};

int sum()
{
    int sumn = 0;
    for(int i=0;i<10;i++)
        sumn+=count[i];
    return sumn;
}

char *originalDigits(char *sbak)
{
    int l = strlen(sbak);
    char *s = malloc(l);
    memcpy(s,sbak,l+1);
    for(int i=0;i<10;i++)
    {
        while(szexistQ(s,l,numsz[i]))
        {
            dropsz(s,l,numsz[i]);
            count[sz2n[i]]++;
        }
    }
    //puts("");
    char *ret = (char *)calloc(sum()+1,1);
    int p = 0;
    for(int i=0;i<10;i++)
        for(int j=count[i];j>0;j--)
        {
            ret[p] = 48+i;
            p++;
        }
    
    return ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

