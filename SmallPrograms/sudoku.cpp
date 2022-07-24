#include <cstdio>
#include <cstdlib>
using namespace std;

int source[81] =
{
    0, 0, 6, 0, 0, 0, 0, 8, 4,
    0, 8, 0, 9, 1, 0, 0, 0, 0,
    0, 0, 7, 2, 0, 0, 9, 0, 0,
    0, 0, 0, 0, 0, 1, 4, 7, 5,
    8, 0, 1, 0, 0, 7, 0, 3, 0,
    2, 7, 0, 0, 9, 5, 0, 0, 0,
    1, 0, 0, 7, 3, 0, 0, 0, 0,
    0, 6, 0, 1, 0, 9, 3, 0, 8,
    3, 0, 0, 0, 0, 0, 7, 0, 6
};

//int table[9] = {1,2,3,4,5,6,7,8,9};

class DET
{
public:
    int dat[8];
    int addin(int);
    int print_dat();
    int refresh()
    {
        DET::p=0;
        return 0;
    }
private:
    int p = 0;
};
int DET::addin(int num)
{
    DET::dat[p] = num;
    p++;
    return 0;
}
int DET::print_dat()
{
    for (int i = 0; i<8; i++)
        putchar(dat[i] + 48), putchar(' ');
    puts("");
    return 0;
}


DET block(int row, int col)
{
    int r = row / 3;
    int c = col / 3;
    DET rv;
    for (int ir = r * 3; ir<r * 3 + 3; ir++)
        for (int ic = c * 3; ic<c * 3 + 3; ic++)
            if (ir != row || ic != col)
                rv.addin(source[ir * 9 + ic]);
    return rv;
}
DET line(int row, int col)
{
    DET rv;
    for(int i=0;i<9;i++)
        if(i!=col)
            rv.addin(source[row*9+i]);
    return rv;
}
DET column(int row, int col)
{
	DET rv;
    for(int i=0;i<9;i++)
        if(i!=row)
            rv.addin(source[i*9+col]);
    return rv;
}

class PV
{
public:
    int init(int,int);
    int get()
    {
        if(temp>len)
        {
            fputs("Error: PV::get: Segment fault!",stderr);
            exit(-1);
        }
        return values[temp++];
    }
private:
    int temp = 0;
    int len = 0;
    int values[9] = {0};
};
int PV::init(int row,int col)
{
    int table[9] = {1,2,3,4,5,6,7,8,9};
    DET cat = block(row,col);
    for(int i=0;i<8;i++)
        if(cat.dat[i]>0&&cat.dat[i]<10)
            for(int j=0;j<9;j++)
                if(table[j]>0&&table[j]<10&&table[j]==cat.dat[i])
                    table[j]=0;
    cat.refresh();                
    cat = line(row,col);
    for(int i=0;i<8;i++)
        if(cat.dat[i]>0&&cat.dat[i]<10)
            for(int j=0;j<9;j++)
                if(table[j]>0&&table[j]<10&&table[j]==cat.dat[i])
                    table[j]=0;
    cat.refresh();                 
    cat = column(row,col);
    for(int i=0;i<8;i++)
        if(cat.dat[i]>0&&cat.dat[i]<10)
            for(int j=0;j<9;j++)
                if(table[j]>0&&table[j]<10&&table[j]==cat.dat[i])
                    table[j]=0; 
    for(int i=0;i<9;i++)
        if(table[i]!=0)
            PV::values[PV::len]=table[i],PV::len++;
    return 0;
}

int main()
{
    PV finaltab[81];
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
            finaltab[i*9+j].init(i,j);
        }
    //初始化完成，调用PV请用get，不接受参数，get会自增，调用者需要判断get要调用几次
    
    return 0;
}