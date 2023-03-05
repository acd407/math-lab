#include <music.h>

int main (int argc, char** argv) {
    if(argc==2) {
        music mu(argv[1]);
        mu.print();
        mu.play();
    }
    return 0;
}