#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char token; //当前处理的字符

double polynomial(void);//多项式 15*8 + 35*(4+2) - 12/(5-3)*9
double term(void);      //多项式的项 35*(4+8)/(4+2)
double factor(void);    //确定的数或因式 15 (35*48+2/5) (polynomial)

char getPrintableChar(void) {
    char temp;
    do
        temp = getchar();
    while (temp==' ');
    return temp;
}

void error(void) {
    fprintf(stderr, "Error!\n");
    exit(EXIT_FAILURE);
}

void match(char expectedToken) {    //匹配检查，并向后读
    if (expectedToken == token)
        token = getPrintableChar();
    else
        error();
}

int main(void)
{
    double result;
    printf(">>: ");
    while(1) {
        token = getPrintableChar();
        result = polynomial();
        if (token == '\n')
            printf("<<< %g\n>>: ", result);
        else
            error();
    }
    return 0;
}

double polynomial(void)
{
    double temp = term();
    while (token == '+' || token == '-')
        switch(token) {
        case '+':
            match('+');
            temp += term();
            break;
        case '-':
            match('-');
            temp -= term();
            break;
        }
    return temp;
}

double term(void)
{
    double temp = factor();
    while (token == '*' || token == '/')
        switch(token) {
        case '*':
            match('*');
            temp *= factor();
            break;
        case '/':
            match('/');
            temp /= factor();
            break;
        }
    return temp;
}

double factor(void)
{
    double temp;
    if (token == '(') {
        match('(');
        temp = polynomial();
        match(')');
    }
    else if (isdigit(token)) {
        ungetc(token, stdin);
        scanf("%lf", &temp);
        token = getPrintableChar();
    }
    else
        error();
    return temp;
}
