%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z]
IDENTIFIER (_|{LETTER})({DIGIT}|{LETTER}|_)*
SELECT     [Ss][Ee][Ll][Ee][Cc][Tt]
FROM       [Ff][Rr][Oo][Mm]
WHERE      [Ww][Hh][Ee][Rr][Ee]

%%
{SPACE}      {}
{SELECT}     { return TOKEN_SELECT; }
{FROM}       { return TOKEN_FROM; }
{WHERE}      { return TOKEN_WHERE; }
","          { return TOKEN_COMMA; }
";"          { return TOKEN_SEMICOLON; }
"*"          { return TOKEN_WILDCARD; }
{IDENTIFIER} { return TOKEN_IDENTIFIER; }
%%

int yywrap() { return 1; }