%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z]
IDENTIFIER (_|{LETTER})({DIGIT}|{LETTER}|_)*
TEXT       \"({DIGIT}|{LETTER}|{SPACE})*\"

%%
{SPACE}      { /* Ignore */ }
"="          { return TOKEN_ASSIGN; }
"¬"          { return TOKEN_NOT; }
"^"          { return TOKEN_AND; }
"v"          { return TOKEN_OR; }
"=>"         { return TOKEN_IMPL; }
"<=>"        { return TOKEN_BICOND; }
"("          { return TOKEN_LPAREN; }
")"          { return TOKEN_RPAREN; }
"eval"       { return TOKEN_EVAL; }
{IDENTIFIER} { return TOKEN_IDENTIFIER; }
{TEXT}       { return TOKEN_STRING; }
%%

int yywrap() { return 1; }