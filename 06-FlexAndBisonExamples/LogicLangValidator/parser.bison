%{
#include <iostream>

extern int yylex();
extern char* yytext;
int yyerror(const char*);
%}

%token TOKEN_STRING
%token TOKEN_NOT
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_IMPL
%token TOKEN_BICOND
%token TOKEN_ASSIGN
%token TOKEN_IDENTIFIER
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_EVAL

%%
program : statement
        ;

statement : assignment statement
          | proposition statement
          | model_evaluation statement
          | assignment
          | proposition
          | model_evaluation
          ;

model_evaluation : TOKEN_EVAL TOKEN_IDENTIFIER
                 ;

assignment : TOKEN_IDENTIFIER TOKEN_ASSIGN TOKEN_STRING 
           | TOKEN_IDENTIFIER TOKEN_ASSIGN proposition
           ;

proposition : proposition TOKEN_AND term
            | proposition TOKEN_OR term
            | proposition TOKEN_IMPL term
            | proposition TOKEN_BICOND term
            | term
            ;

term : TOKEN_IDENTIFIER
     | TOKEN_NOT term
     | TOKEN_LPAREN proposition TOKEN_RPAREN
     ;
%%

int yyerror(const char* s)
{
    std::cout << "Parse error: " << s << std::endl;
    return 1;
}