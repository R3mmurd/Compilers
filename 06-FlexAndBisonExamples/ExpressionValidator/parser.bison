%{
#include <stdio.h>

extern int yylex();
int yyerror(const char*);
%}

%token TOKEN_INT
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_LPAREN
%token TOKEN_RPAREN

%%
program : expr;

expr : expr TOKEN_PLUS term
     | expr TOKEN_MINUS term
     | term
     ;

term : term TOKEN_MUL factor
     | term TOKEN_DIV factor
     | term TOKEN_MOD factor
     | factor
     ;

factor : TOKEN_MINUS factor
       | TOKEN_LPAREN expr TOKEN_RPAREN
       | TOKEN_INT
       ;
%%

int yyerror(const char* s)
{
    printf("Parse error: %s\n", s);
    return 1;
}