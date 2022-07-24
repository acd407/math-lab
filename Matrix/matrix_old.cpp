#include <iostream>
#include <fstream>
#include <iomanip>      //Needed for setiosflags(ios::fixed)
#include <algorithm>    //Needed for swap
#define X colnum
#define Y rownum
using namespace std;

class MATRIX
{
public:
    void InitializeSize(int row,int col);//输入真正的行列数
    void Remove(void);
    void Transpose(void);
    void RowOperate(int sourcerow, int targetrow, double times);//输入真正的行列数
    void ColumnOperate(int sourcecolumn, int targetcolumn, double times);//输入真正的行列数
    void SetAValue(int row, int col, double value);//输入真正的行列数
    void InitializeValues(FILE *_fp);
    void InitializeValues(double *value);
    void PrintValues(int precision);
    double Det(bool backup);
    double Det(void);//Enhance
    void SmallValueCheckUp(void);
    bool GetSignal(void);
    void MATMUL(double *Matrix2,int length,int width);
    void NUMMUL(double times);
private:
    double *Matrix;
    int rownum;
    int colnum;
    bool signal = 1; //1 is +,0 is -
    void RowExchange(int row_0, int row_1);
    void ColumnExchange(int column_0, int column_1);
};
bool MATRIX::GetSignal(void)
{
    return signal;
}
void MATRIX::RowOperate(int sourcerow, int targetrow, double times)
{
    sourcerow--;
    targetrow--;
    if(times==0)
        MATRIX::RowExchange(sourcerow, targetrow);
    if (sourcerow >= 0 && sourcerow < Y)
        for (int i = 0; i < X; i++)
            Matrix[targetrow * X + i] += times * Matrix[sourcerow * X + i];
    else
        for (int i = 0; i < X; i++)
            Matrix[targetrow * X + i] *= times;
}
void MATRIX::RowExchange(int row_0, int row_1)
{
    for (int i = 0; i < X; i++)
        swap(Matrix[row_0 * X + i], Matrix[row_1 * X + i]);
    signal = !signal;
}
void MATRIX::ColumnOperate(int sourcecolumn, int targetcolumn, double times)
{
    sourcecolumn--;
    targetcolumn--;
    if(times==0)
        MATRIX::ColumnExchange(sourcecolumn, targetcolumn);
    else if (sourcecolumn >= 0 && sourcecolumn < X)
        for (int i = 0; i < Y; i++)
            Matrix[i * X + targetcolumn] += times * Matrix[i * X + targetcolumn];
    else
        for (int i = 0; i < Y; i++)
            Matrix[i * X + targetcolumn] *= times;
}
void MATRIX::ColumnExchange(int column_0, int column_1)
{
    for (int i = 0; i < Y; i++)
            swap(Matrix[i * X + column_0], Matrix[i * X + column_1]);
    signal = !signal;
}
void MATRIX::Transpose()
{
    double tempmat[X*Y];
    for(int i=0;i<X*Y;i++)
        tempmat[i]=Matrix[i];
    for(int i=1;i<=X;i++)
        for(int j=1;j<=Y;j++)
            Matrix[(i-1)*Y+j-1]=tempmat[(j-1)*X+i-1];//Be Careful !
    swap(colnum,rownum);
}
void MATRIX::InitializeSize(int row,int col)
{
    rownum = row;
    colnum = col;
    Matrix = new double[colnum * rownum];
}
void MATRIX::Remove()
{
    delete[] Matrix;
    colnum = 0;
    rownum = 0;
    Matrix = NULL;
}
double MATRIX::Det(bool backup)
{
    if(X!=Y)
    {
        cerr<<"Det : error : Only when RowNumber equals ColumnNumber can calculate be continued.";
        exit(-1);
    }
    double temp[X*Y];
    double ret = 1;
    //备份Matrix
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            temp[i * X + j] = Matrix[i * X + j];
    
    if (0 == Matrix[0])
        for (int i = 1; i < Y; i += X)
            if (Matrix[i] != 0)
                MATRIX::RowExchange(0, i);
    //计算行列式
    for (int t = 1; t < Y; t++)
        for (int s = 0; s < t; s++)
        {
            if (Matrix[s * X + s] < 1e-10)
            {
                for (int i = s + 1; i < Y; i++)
                    if (Matrix[i * X + s] != 0)
                    {
                        MATRIX::RowExchange(s, i);
                        goto retry;
                    }
                goto error;
            }
        retry:
            MATRIX::RowOperate(s, t, -Matrix[t * X + s] / Matrix[s * X + s]);
        }
    //统计
    for (int i = 0; i < X; i++)
        ret *= Matrix[i * X + i];
    //还原Matrix
    if (backup == 1)
        for (int i = 0; i < Y; i++)
            for (int j = 0; j < X; j++)
                Matrix[i * X + j] = temp[i * X + j];
    //添加正负号
    if (signal == 0)
        ret = -ret;
    return ret;
error:
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            Matrix[i * X + j] = temp[i * X + j];

    cerr << "Det : error !\nRetry ? [y]/n : ";
    if (getc(stdin) == '\n' || getc(stdin) == 'y' || getc(stdin) == 'Y')
    {
        MATRIX::Transpose();
        return MATRIX::Det(backup);
    }
    else if (getc(stdin) == 'n' || getc(stdin) == 'N')
        _exit(-1);
    else
    {
        cerr << "Det : input error !";
        exit(-1);
    }
}
void MATRIX::SetAValue(int row, int col, double value)
{
    row--;
    col--;
    Matrix[row * X + col] = value;
}
void MATRIX::InitializeValues(FILE *_fp)
{
    //不知道>>输入时如何去掉","，就用FILE*吧
    /*ifstream fp;
	fp.open(filename);
	for(int i = 0; i<order; i++)
	for(int j = 0; j<order; j++)
	fp >> Matrix[i][j];
	fp.close();*/
    fseek(_fp, 0, SEEK_SET);
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            fscanf(_fp, "%lf,", &(Matrix[i * X + j]));
    if(fgetc(_fp)!=EOF)
        cerr<<"InitializeValues(from file) : Waring : Some data may be ignored.";
}
void MATRIX::InitializeValues(double *value)
{
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            Matrix[i * X + j] = value[i * X + j];
}
void MATRIX::PrintValues(int precision)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
            cout << setprecision(precision) << Matrix[i * X + j] << '\t';
        cout << '\n';
    }
}
void MATRIX::SmallValueCheckUp(void)
{
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            if (Matrix[i * X + j] < 1e-10 && Matrix[i * X + j] > -1e-10)
                MATRIX::SetAValue(i, j, 0);
}
void MATRIX::MATMUL(double *Matrix2,int length,int width)
{
    if(width!=X)
    {
        cerr<<"MATMUL : error : Width of Matrix2 must be equal to length of Matrix.";
        exit(-1);
    }
    int X_Bak = X;
    double Matrix_Bak[X*Y];
    for(int i=0;i<Y;i++)
        for(int j=0;j<X;j++)
            Matrix_Bak[i*X+j]=Matrix[i*X+j];
    delete[] Matrix;
    X = length;
    Matrix = new double[X*Y];
    for(int i=0;i<Y;i++)
        for(int j=0;j<X;j++)
        {
            Matrix[i*X+j]=0;
            for(int k=0;k<X_Bak;k++)
                Matrix[i*X+j]+=Matrix_Bak[i*X_Bak+k]*Matrix2[k*length+j];
        }
}
void MATRIX::NUMMUL(double times)
{
    for(int i=0;i<Y;i++)
        for(int j=0;j<X;j++)
            Matrix[i*X+j]*=times;
}
int main()
{
    cout << setiosflags(ios::fixed); //保证setprecision()是设置小数点后的位数。
    double mat[20]={5,5,3,2,4,
                    8,7,6,0,4,
                    2,2,4,5,1,
                    6,6,4,3,7};
    double mat2[15] =  {2,5,3,
                        4,8,9,
                        6,4,5,
                        0,2,4,
                        0,1,7};                
    MATRIX Mat;
    Mat.InitializeSize(4,5);//记得对应着要改
    Mat.InitializeValues(mat);
    Mat.MATMUL(mat2,3,5);
    Mat.PrintValues(2);
    cout<<sizeof(mat2);
    return 0;
}