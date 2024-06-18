%{
#include <stdio.h>
%}

%%
hello { printf("Hello, World!\n"); }
bye { printf("Goodbye, World!\n"); }
%%

int yywrap()
{
    return 1;
}