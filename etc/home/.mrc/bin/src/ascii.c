#include <stdio.h>
#include <string.h>
int main() {
    char *control[32] = {
        "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", 
        "BS" , "HT" , "LF" , "VT" , "FF" , "CR" , "SO" , "SI" ,
        "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
        "CAN", "EM" , "SUB", "ESC", "FS" , "GS" , "RS" , "US" ,
    };
    for(int i=0;i<32;i++)
        printf("%d %x %s\n", i, i, control[i]);
    for(int i=32;i<128;i++)  
        printf("%d %x %c\n", i, i, i); 
    return 0; 
}
