#include <stdlib.h>
#include <stdio.h>
#include <string>
using std::string;

#define LOWER(x) ((x)<96?((x)+32):(x))
#define UPPER(x) ((x)>96?((x)-32):(x))

int dirlen(const char *str)
{
    int i=0;
    for(;str[i];i++) {
        if(str[i]=='/'||str[i]==':'||!str[i])
            break;
    }
    return i;
}

// int dirlen(const char *str)
// {
//     int i=0;
//     for(;str[i];i++) {
//         if(str[i]=='/'||str[i]==':'||!str[i])
//             break;
//     }
//     return i;
// }

int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("usage: %s str (needed to be converted).", argv[0]);
        return 0;
    }
    string str(argv[1]);
    for(int i=0;i<str.length();i++) {
        //puts(str.c_str());
        if(str[i]=='/') {
            if(str[i-1]==';'||i==0) {   //需要头转换
                if(!str[i+1]||dirlen(str.c_str()+i+1)!=1) { // /usr/bin
                    str.insert(i,"D:\\MSYS2");
                    i += 7;
                } else if (str[i+1]&&dirlen(str.c_str()+i+1)==1) {  // /c/windows
                    str[i] = UPPER(str[i+1]);
                    str[i+1] = ':';
                    i += 1;
                }
            } else {
                str[i] = '\\';
            }
        } 
        else if(str[i]==':') {
            str[i] = ';';
        }
    }
    printf("%s",str.c_str());
    return 0;
}