%{
#include <stdio.h>
#include <expression.hpp>

#define YYSTYPE Expression*

extern int yylex();
extern char* yytext;
int yyerror(const char*);
Expression* parser_result{nullptr};
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

expr : expr TOKEN_PLUS term              { $$ = new Addition($1, $3); }
     | expr TOKEN_MINUS term             { $$ = new Subtraction($1, $3); }
     | term                              { $$ = $1; }
     ;

term : term TOKEN_MUL factor             { $$ = new Multiplication($1, $3); }
     | term TOKEN_DIV factor             { $$ = new Division($1, $3); }
     | term TOKEN_MOD factor             { $$ = new Module($1, $3); }
     | factor                            { $$ = $1; }
     ;

factor : TOKEN_MINUS factor              { $$ = new Subtraction(new Value(0), $2); }
       | TOKEN_LPAREN expr TOKEN_RPAREN  { $$ = $2; }
       | TOKEN_INT                       { $$ = new Value(atoi(yytext)); }
       ;
%%

int yyerror(const char* s)
{
    printf("Parse error: %s\n", s);
    return 1;
}