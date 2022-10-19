#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

//! link: stdc++
//! flag: -O3 -s -D WIN
//! int: /tmp
//! out: ../setluap

#ifdef WIN
    #define STRCUT ';'
    #define STRDIR '\\'
#elifdef MSYS
    #define STRCUT ':'
    #define STRDIR '/'
#endif

int main(int , char **, char **)
{
    char *str = std::getenv("LUA_CPATH");
    if(!str) {
        std::cerr<<"get env failure. exiting..."<<std::endl;
        return 0;
    }
    std::vector<std::string> strlist;
    int k=0;
    for(int i=0;i<std::strlen(str);i++)
        if(str[i]==STRCUT) {
            strlist.push_back(std::string(str+k, i-k+1));
            if(*(strlist.back().end()-2)==STRDIR)
                strlist.back().insert(strlist.back().size()-1, "?.dll");
            else {
                std::cerr<<"PATH_str format error!"<<std::endl;
                return 0;
            }
            k = i+1;
        }
    strlist.back().push_back(STRCUT);
    for(auto& i:strlist)
        std::cout<<i;
    return 0;
}