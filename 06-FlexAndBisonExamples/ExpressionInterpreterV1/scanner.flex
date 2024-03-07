%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
INT_NUMBER {DIGIT}+

%%
{SPACE}      {}
"+"          return TOKEN_PLUS;
"-"          return TOKEN_MINUS;
"*"          return TOKEN_MUL;
"/"          return TOKEN_DIV;
"%"          return TOKEN_MOD;
"("          return TOKEN_LPAREN;
")"          return TOKEN_RPAREN;
{INT_NUMBER} return TOKEN_INT;
%%

int yywrap() { return 1; }