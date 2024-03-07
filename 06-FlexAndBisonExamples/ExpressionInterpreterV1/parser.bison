%{
#include <stdio.h>

extern int yylex();
extern char* yytext;
int yyerror(const char*);
int parser_result;
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
program : expr                           { parser_result = $1; }
        ;

expr : expr TOKEN_PLUS term              { $$ = $1 + $3; }
     | expr TOKEN_MINUS term             { $$ = $1 - $3; }
     | term                              { $$ = $1; }
     ;

term : term TOKEN_MUL factor             { $$ = $1 * $3; }
     | term TOKEN_DIV factor             { $$ = $1 / $3; }
     | term TOKEN_MOD factor             { $$ = $1 % $3; }
     | factor                            { $$ = $1; }
     ;

factor : TOKEN_MINUS factor              { $$ = -$2; }
       | TOKEN_LPAREN expr TOKEN_RPAREN  { $$ = $2; }
       | TOKEN_INT                       { $$ = atoi(yytext); }
       ;
%%

int yyerror(const char* s)
{
    printf("Parse error: %s\n", s);
    return 1;
}