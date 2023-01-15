#include <stdio.h>
#include <Windows.h>
#define true 1
#define false 0

void help () {
    puts("\tC\tC#\tD\tD#\tE\tF\tF#\tG\tG#\tA\tA#\tB");
    for(int r=0;r<=10;r++) {
        printf("%d\t", r);
        for(int c=0;c<=11&&(12 * r + c <= 127);c++)
            printf("%d\t", 12 * r + c);
        putchar('\n');
    }
}

void print(int d) {
    HMIDIOUT handle;
    midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
    midiOutShortMsg(handle, (0x007f << 16) + (d << 8) + 0x90);
    Sleep(1000);
    midiOutClose(handle);
}

int main (int argc, char **argv) {
    int status = true;
    if (argc == 2) {
        for (int i=0;argv[1][i];i++)
            if (argv[1][i]<'0'||argv[1][i]>'9')
                status = false;
    } else {
        help();
        return 0;
    }
    if (status) {
        int d = atoi(argv[1]);
        printf("%d\n", d);
        print(d);
        
    } else
        puts("input error.");
    return 0;
}