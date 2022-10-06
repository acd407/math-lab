#include <stdio.h>
#include "uint256.h"
int main()
{
    uint128_t a = {0x2f00ff00ff00ff00, 13};
    uint128_t b = {0, 17};
    uint128_t d, m, t;
    char str[32];
    if(tostring128(&a, 16, str, 32))
        printf("%s\n", str);
    printf("%d\n", bits128(&a));
    printf("%d\n", gte128(&a, &b));
    divmod128(&a, &b, &d, &m);
    if(tostring128(&d, 16, str, 32))
        printf("%s\n", str);
    if(tostring128(&m, 16, str, 32))
        printf("%s\n", str);
    mul128(&d, &b, &t);
    if(tostring128(&t, 16, str, 32))
        printf("%s\n", str);
}