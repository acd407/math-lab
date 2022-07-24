#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
class det
{   
public:
    vector<vector<double> > data;
    double sign = 1;
    //vector<vector<double> > *data;
    det(){};
    det(double *initdata,int length)
    {
        int order = sqrt(length);
        if(order*order!=length)//检测数组是否为平方数
        {
            cerr<<"error: length is incorrect, please check it again.";
            exit(__LINE__);
        }
        //拷贝
        for(int i=0;i<order;i++)
        {
            vector<double> bar(initdata+order*i,initdata+order*(i+1));
            data.push_back(bar);
            bar.clear();
        }
    }
    ~det()
    {
        for(auto i : data)
            i.clear();
        data.clear();
    }
//======START======START======START======START======START======START======
    void transpose()
    {
        int order = data.size();
        for(int i=0;i<order;i++)
            for(int j=0;j<i;j++)
                swap(data[i][j],data[j][i]);
    }
    void print()
    {
        for(auto i:data)
        {
            for(auto j:i)
                cout<<j<<'\t';
            cout<<endl;
        }
    }
    void swaprow(int i,int j)
    {
        swap(data[i],data[j]);
        sign = -sign;
    }
    void timesrow(int row,double times)
    {
        for(auto i:data[row])
            i *= times;
    }
    void addrows(int target,int source,double times)
    {
        int order = data.size();
        for(int i=0;i<order;i++)
            data[target][i] += data[source][i] * times;
    }
    void smallnumcheck()
    {
        for(auto &i:data)
            for(auto &j:i)
                if(abs(j)<1e-5)
                    j=0;
    }
    void toUTM()
    {
        int order = data.size();
        for(int j=1;j<order;j++)
        {
            //错误处理
            if(data[j-1][j-1]==0)
            {
                bool error = true;
                //向下寻找
                for(int k=j-1;k<order;k++)
                {
                    if(data[k][j-1]!=0)
                    {
                        swaprow(j-1,k);
                        error = false;
                    }
                }
                if(error)
                {
                    cerr<<"error: can't resolve this mistake.\nTry to transpose the det.";
                    exit(__LINE__);
                }          
            }
            //计算
            for(int i=j;i<order;i++)
            {
                addrows(i,j-1,-data[i][j-1]/data[j-1][j-1]);
            }
            smallnumcheck();
        }
    }
    void toSM()
    {
        int order = data.size();
    }
    double calc()
    {
        int order = data.size();
        double retvalue = 1;
        for(int i=0;i<order;i++)
            retvalue *= data[i][i];
        return sign*retvalue;
    }
//========END========END========END========END========END========END========
};
int main()
{
    double temp[16];
    for(int i=0;i<16;i++)
        cin>>temp[i];
    det test(temp,16);
    //test.transpose();
    test.toUTM();
    test.print();
    cout<<test.calc();
}