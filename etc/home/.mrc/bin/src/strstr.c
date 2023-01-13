#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if(argc!=3) {
        printf("%s source_string patton_string", argv[1]);
        return 0;
    }
    if(strstr(argv[1], argv[2])) {
        printf("%s", argv[2]);
    }
    return 0;
}