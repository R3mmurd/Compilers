%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z]
IDENTIFIER (_|{LETTER})({DIGIT}|{LETTER}|_)*
INTEGER    (0|[1-9]{DIGIT}*)

%%
{SPACE}      {}
"if"         { return TOKEN_IF; }
"then"       { return TOKEN_THEN; }
"else "      { return TOKEN_ELSE; }
"="          { return TOKEN_EQUAL; }
"<"          { return TOKEN_LESS; }
"<="         { return TOKEN_LESS_EQ; }
">"          { return TOKEN_GREATER; }
">="         { return TOKEN_GREATER_EQ; }
"<>"         { return TOKEN_NOT_EQUAL; }
"andalso"    { return TOKEN_AND; }
"orelse"     { return TOKEN_OR; }
"not"        { return TOKEN_NOT; }
"~"          { return TOKEN_NEG; }
{INTEGER}    { return TOKEN_INT; }
{IDENTIFIER} { return TOKEN_IDENTIFIER; }
%%

int yywrap() { return 1; }