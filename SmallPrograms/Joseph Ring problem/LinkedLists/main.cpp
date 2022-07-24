#include <iostream>
#include <fstream>
#define NEXT 1
#define BACK 0
using namespace std;

struct line
{
	int data;
	line *Next;
	line *Last;
};
class Linkedlist
{
	public:
	line *hCurrent;
	int CreatOne(void);
	int Delete(void);
	int PrintAllData(void);
	int SetData(int num);
	int Init(int,int,int);
	int Go(bool step);
	Linkedlist()
	{
		Linkedlist::Init(1,2,3);
	}
	~Linkedlist();
};
Linkedlist::~Linkedlist()
{
	line *hStart=hCurrent->Last;
	do
	{
		hCurrent=hCurrent->Next;
		delete[] hCurrent->Last;
		
	} while (hCurrent!=hStart);
	delete[] hCurrent;
}
int Linkedlist::Init(int num0,int num1,int num2)
{
	line *hpn = NULL, *hpl = NULL;
	hCurrent = new line;
	hpn = new line;
	hpl = new line;
	hCurrent->Next = hpn;
	hCurrent->Last = hpl;
	hpn->Next = hpl;
	hpn->Last = hCurrent;
	hpl->Next = hCurrent;
	hpl->Last = hpn;
	hCurrent->data=num1;
	hpn->data=num2;
	hpl->data=num0;
	return 0;	
}
int Linkedlist::SetData(int num)
{
	hCurrent->data=num;
	return 0;
}
int Linkedlist::PrintAllData(void)
{
	cout<<hCurrent->data<<'\t';
	line * Start = hCurrent;
	hCurrent = hCurrent->Next;
	while(hCurrent!=Start)
	{
		//cout<<"int : "<<hCurrent->data<<'\n';
		cout<<hCurrent->data<<'\t';
		hCurrent = hCurrent->Next;
	}
	return 0;
}
int Linkedlist::Go(bool step)
{
	if(step==0)
	hCurrent=hCurrent->Last;
	else if(step==1)
	hCurrent=hCurrent->Next;
	return 0;
}
int Linkedlist::Delete(void)	//自动移到下一个链节
{
	hCurrent = hCurrent->Next;
	line *LastBak = hCurrent->Last->Last;	

	delete[] hCurrent->Last;
	hCurrent->Last = LastBak;
	hCurrent->Last->Next = hCurrent;
	return 0;
}
int Linkedlist::CreatOne(void)
{
	line *LastBak = hCurrent->Next;				//保存下一个链节的位置
	hCurrent->Next = new line;				//创建下一个链节
	if(hCurrent->Next == NULL)return 1;
	
	hCurrent->Next->Last = hCurrent;			//完善新建的链节信息
	hCurrent->Next->Next = LastBak;
	
	hCurrent->Next->Next->Last = hCurrent->Next;
	return 0;
}
int main(int argc,char **argv)
{
	if(argc!=3)
	{
		cerr<<"arguments error!";
		exit(-1);
	}
	int step=atoi(argv[2]);
	int total=atoi(argv[1]);
	if(step>1000)
	{
		cerr<<"step is to long!";
		exit(-1);
	}
	Linkedlist l;
	l.Go(NEXT);

	for(int i=4;i<=total;i++)
	{
		l.CreatOne();
		l.Go(NEXT);
		l.SetData(i);
	}
	l.Go(NEXT);
	while(l.hCurrent!=l.hCurrent->Next)
	{
		for(int i=1;i<step;i++)
			l.Go(NEXT);
		l.Delete();//Delete自带Go的功能，3个Go实际上前进了4步
	}
	l.PrintAllData();
	return 0;
}