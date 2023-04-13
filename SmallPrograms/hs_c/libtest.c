//gcc -c libtest.c -I /usr/lib/ghc-9.0.2/include/
//gcc libtest.o /f/Desktop/mylib.so /usr/lib/ghc-9.0.2/rts/libHSrts-ghc9.0.2.so
//LD_LIBRARY_PATH=/f/Desktop/mylib.so:/usr/lib/ghc-9.0.2/rts/:$LD_LIBRARY_PATH ./a.out
#include "fs_stub.h"

int main (int argc, char *argv[]) {
	hs_init(&argc, &argv);
	demo();
	hs_exit();
}