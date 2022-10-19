#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

int main()
{
    char *PATH = getenv("PATH");
    vector<string> strlist;
    int k=0;
    for(int i=0;i<strlen(PATH);i++)
        if(PATH[i]==':') {
            strlist.push_back(string(PATH+k, i-k+1));
            k = i+1;
        }
    strlist.push_back(PATH+k);
    
    int env = 0;
    string *str1 = NULL, *str2 = NULL;
    for(auto& i:strlist) {
        if(i==string("/usr/bin/:")) {
            str1 = &i;
            if(!env) {
                env = 1;
            }
        }
        else if(i==string("/mingw64/bin/:")) {
            str2 = &i;
            if(!env) {
                env = 2;
            }
        }
    }
    if(str1&&str2)
        str1->swap(*str2);
    else
        cerr<<"PATH error!"<<endl;
    
    if(env==1)
        cerr<<"Environment: MINGW64"<<endl;
    else if(env==2)
        cerr<<"Environment: MSYS2"<<endl;

    for(auto i:strlist)
        cout<<i;
        
    return 0;
}