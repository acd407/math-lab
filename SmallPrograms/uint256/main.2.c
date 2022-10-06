#include <stdio.h>
#include "uint256.h"
int main()
{
    uint8_t *buf = "cefgthuj48953217";
    uint128_t ui128;
    readu128BE(buf, &ui128);
    char str[32];
    if(tostring128(&ui128, 16, str, 32))
        printf("%s\n", str);
}