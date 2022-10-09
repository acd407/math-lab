#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::strlen;
using std::find;

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
    
    std::sort(strlist.begin(), strlist.end());
    if(strlist.size()>1)
        for(auto i=strlist.begin()+1;i!=strlist.end();i++)
            if(*i==*(i-1))
                strlist.erase(i--);

    for(auto i=0;i<strlist.size();i++)
        cout<<strlist.at(i);

    return 0;
}