#include <stdio.h>
#include <stdlib.h>

extern int yylex();

int main(int argc, char* argv[])
{
    while (1)
    {
        yylex();
    }

    return 0;
}