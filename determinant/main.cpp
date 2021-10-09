#include <iostream>
#include <fstream>
using namespace std;

class DETERMINANT
{
public:
	void InitializeSize(int order);
	void Remove(void);
	void Transpose(void);
	void RowOperate(int objectrow, int targetrow, double times);
	void ColumnOperate(int objectcolumn, int targetcolumn, double times);
	void SetAValue(int row, int col, double value);
	void InitializeValues(FILE *_fp);
	void PrintValues(void);
	double Calculate(bool backup);
	void SmallValueCheckUp(void);
private:
	double **Table;
	int order;
};
void DETERMINANT::RowOperate(int objectrow, int targetrow, double times)
{
	for (int i = 0; i<order; i++)
		Table[targetrow][i] += times*Table[objectrow][i];
}
void DETERMINANT::ColumnOperate(int objectcolumn, int targetcolumn, double times)
{
	for (int i = 0; i<order; i++)
		Table[targetcolumn][i] += times*Table[objectcolumn][i];
}
void DETERMINANT::Transpose()
{
	double temp;
	for (int i = 0; i<order; i++)
		for (int j = 0; j<i - 1; j++)
		{
			temp = Table[i][j];
			Table[i][j] = Table[j][i];
			Table[j][i] = temp;
		}
}
void DETERMINANT::InitializeSize(int inorder)
{
	order = inorder;
	Table = new double *[order];
	for (int i = 0; i<order; i++)
	{
		Table[i] = new double[order];
	}
}
void DETERMINANT::Remove()
{
	for (int i = 0; i<order; i++)
		delete[] Table[i];
	delete[] Table;
	order = 0;
	Table = NULL;
}
double DETERMINANT::Calculate(bool backup)
{
	double temp[order*order];
	if(backup==1)	//备份Table
	for (int i = 0; i<order; i++)
		for (int j = 0; j<order; j++)
			temp[i*order+j]=Table[i][j];
			
	//计算行列式
	for (int t = 1; t<order; t++)
		for (int o = 0; o<t; o++)
				DETERMINANT::RowOperate(o, t, -Table[t][o]/Table[o][o]);
	//统计
	double ret = 1;
	for (int i = 0; i<order; i++)
		ret *= Table[i][i];
	
	if(backup==1)	//还原Table
	for (int i = 0; i<order; i++)
		for (int j = 0; j<order; j++)
			Table[i][j]=temp[i*order+j];

	return ret;
}
void DETERMINANT::SetAValue(int row, int col, double value)
{
	Table[row][col] = value;
}
void DETERMINANT::InitializeValues(FILE *_fp)
{
	//不知道>>输入时如何去掉","，就用FILE*吧
	/*ifstream fp;
	fp.open(filename);
	for(int i = 0; i<order; i++)
	for(int j = 0; j<order; j++)
	fp >> Table[i][j];
	fp.close();*/
	fseek(_fp, 0, SEEK_SET);
	for (int i = 0; i<order; i++)
		for (int j = 0; j<order; j++)
			fscanf(_fp, "%lf,", &(Table[i][j]));
}
void DETERMINANT::PrintValues(void)
{
	for (int i = 0; i<order; i++)
	{	
		for (int j = 0; j<order; j++)
			cout<<Table[i][j]<<'\t';
		cout<<'\n';
	}
}
void DETERMINANT::SmallValueCheckUp(void)
{
	for (int i = 0; i<order; i++)
		for (int j = 0; j<order; j++)
			if(Table[i][j]<1e-10&&Table[i][j]>-1e-10)
				DETERMINANT::SetAValue(i, j, 0);
}

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		puts("Arguments Error!");
		exit(-1);
	}
	FILE *fp = fopen(argv[1], "r");
	int n = 0;
	char ch = fgetc(fp);
	while ('\n' != ch)
	{
		if (ch == ',')
			n++;
		ch = fgetc(fp);
	}
	DETERMINANT Det;
	Det.InitializeSize(n+1);
	Det.InitializeValues(fp);
	Det.PrintValues();
	cout<<'\n'<<Det.Calculate(0)<<endl;
	Det.SmallValueCheckUp();
	Det.PrintValues();
	fclose(fp);
	return 0;
}
