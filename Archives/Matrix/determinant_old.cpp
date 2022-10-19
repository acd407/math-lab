#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class DETERMINANT
{
public:
	DETERMINANT()
	{
		cerr<<"error: Argument can't be empty!";
		abort();
	}
	DETERMINANT(int inorder)
	{
		order = inorder;
		Table = new double[order * order];
	}
	~DETERMINANT()
	{
		delete[] Table;
		order = 0;
		Table = NULL;
	}
	void Transpose(void);
	void RowOperate(int sourcerow, int targetrow, double times)
	{
		for (int i = 0; i < order; i++)
			Table[targetrow * order + i] += times * Table[sourcerow * order + i];
	}
	void RowExchange(int row_0, int row_1);
	void ColumnOperate(int sourcecolumn, int targetcolumn, double times)
	{
		for (int i = 0; i < order; i++)
			Table[targetcolumn * order + i] += times * Table[sourcecolumn * order + i];
	}
	void ColumnExchange(int column_0, int column_1);
	void SetAValue(int row, int col, double value)
	{
		Table[row * order + col] = value;
	}
	void InitializeValues(FILE *_fp);
	double Calculate(bool backup);
	void PrintValues(void)
	{
		for (int i = 0; i < order; i++)
		{
			for (int j = 0; j < order; j++)
				cout << Table[i * order + j] << '\t';
			cout << '\n';
		}
	}
	void SmallValueCheckUp(void)
	{
		for (int i = 0; i < order; i++)
			for (int j = 0; j < order; j++)
				if (Table[i * order + j] < 1e-10 && Table[i * order + j] > -1e-10)
					SetAValue(i, j, 0);
	}
	void InitializeValues(double value[])
	{
		for (int i = 0; i < order; i++)
			for (int j = 0; j < order; j++)
				Table[i * order + j] = value[i * order + j];
	}
	bool GetSignal(void)
	{
		return signal;
	}
private:
	double *Table;
	int order;
	bool signal=1;	//1 is +,0 is -
	void FirstCellZeroSolution(void)
	{
		for (int i = 1; i < order; i += 4)
			if (Table[i] != 0)
				DETERMINANT::RowExchange(0,i);
	}
};

void DETERMINANT::RowExchange(int row_0, int row_1)
{
	double temp[order];
	for(int i=0;i<order;i++)
	{	
		temp[i]=Table[row_0*order+i];
		Table[row_0*order+i]=Table[row_1*order+i];
		Table[row_1*order+i]=temp[i];
	}
	signal = !signal;
}
void DETERMINANT::ColumnExchange(int column_0, int column_1)
{
	double temp[order];
	for(int i=0;i<order;i++)
	{	
		temp[i]=Table[i*order+column_0];
		Table[i*order+column_0]=Table[i*order+column_1];
		Table[i*order+column_1]=temp[i];
	}
	signal = !signal;
}
void DETERMINANT::Transpose()
{
	double temp;
	for (int i = 0; i < order; i++)
		for (int j = 0; j < i - 1; j++)
		{
			temp = Table[i * order + j];
			Table[i * order + j] = Table[j * order + i];
			Table[j * order + i] = temp;
		}
}

double DETERMINANT::Calculate(bool backup)
{
	double temp[order * order];
	double ret = 1;
	//备份Table
	for (int i = 0; i < order; i++)
		for (int j = 0; j < order; j++)
			temp[i * order + j] = Table[i * order + j];
	if(0==Table[0])
		DETERMINANT::FirstCellZeroSolution();
	//计算行列式
	for (int t = 1; t < order; t++)
		for (int s = 0; s < t; s++)
		{
			if(Table[s * order + s]<1e-10)
			{
				for(int i=s+1;i<order;i++)
					if(Table[i*order+s]!=0)
					{
						DETERMINANT::RowExchange(s,i);
						goto retry;
					}
				goto error;
			}
		retry:
			DETERMINANT::RowOperate(s, t, -Table[t * order + s] / Table[s * order + s]);
		}
	//统计
	for (int i = 0; i < order; i++)
		ret *= Table[i * order + i];
	//还原Table
	if (backup == 1) 
		for (int i = 0; i < order; i++)
			for (int j = 0; j < order; j++)
				Table[i * order + j] = temp[i * order + j];
	//添加正负号
	if(signal == 0)
		ret = -ret;
	return ret;
error:
	for (int i = 0; i < order; i++)
			for (int j = 0; j < order; j++)
				Table[i * order + j] = temp[i * order + j];

	cerr<<"Calculate : error !\nRetry ? [y]/n : ";
	if(getc(stdin)=='\n'||getc(stdin)=='y'||getc(stdin)=='Y')
	{	
		DETERMINANT::Transpose();
		return DETERMINANT::Calculate(backup);
	}
	else if(getc(stdin)=='n'||getc(stdin)=='N')
		_exit(-1);
	else 
	{
		cerr<<"Calculate : input error !";
		exit(-1);
	}
}

void DETERMINANT::InitializeValues(FILE *_fp)
{
	fseek(_fp, 0, SEEK_SET);
	for (int i = 0; i < order; i++)
		for (int j = 0; j < order; j++)
			fscanf(_fp, "%lf,", &(Table[i * order + j]));
}

int main()
{
	DETERMINANT Det(4);	
	double mat[16] = {0, 0, 0, 1, 0, 15, 2, 7, 0, 0, 40, 1, 0, 0, 0, 3};
	Det.InitializeValues(mat);
	Det.PrintValues();
	//Det.SmallValueCheckUp();
	//cout<<Det.Calculate(1)<<endl;
	return 0;
}
