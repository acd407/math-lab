#include <cstring>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    if(argc != 2) {
        std::printf("usage: %s str (needed to be converted).", argv[0]);
        return 0;
    }
    vector<string> strlist;
    int k=0;
    for(int i=0;i<strlen(argv[1]);i++)
        if(argv[1][i]==';') {
            strlist.push_back(string(argv[1]+k, i-k+1));
            k = i+1;
        }
    strlist.push_back(argv[1]+k);
    
    if(strlist.back().back()!=';')
        strlist.back().push_back(';');

    for(auto k=0;k<strlist.size();k++) {
        auto& i = strlist.at(k);
        if(*(i.end()-2)!='\\') {
            i.insert(i.end()-1, '\\');
        }
    }
    
    for(auto i=strlist.begin();i!=strlist.end();i++) {
        for(auto j=i+1;j!=strlist.end();j++) {
            if(*i==*j)
                strlist.erase(j--);
        }
    }

    for(auto i=0;i<strlist.size();i++)
        cout<<strlist.at(i);

    return 0;
}