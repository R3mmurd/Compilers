%{

#define STR(s) #s
#define XSTR(s) STR(s)

typedef enum
{
    TOKEN_EOF = 0,
    TOKEN_SELECT = 258,
    TOKEN_FROM = 259,
    TOKEN_WHERE = 260,
    TOKEN_COMMA = 261,
    TOKEN_WILDCARD = 262,
    TOKEN_IDENTIFIER = 263
}
token_t;

extern FILE* yyin;
extern int yylex();
extern char* yytext;

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
"*"          { return TOKEN_WILDCARD; }
{IDENTIFIER} { return TOKEN_IDENTIFIER; }
%%

int yywrap() { return 1; }

void usage(char *argv[])
{
    printf("Usage: %s input_file\n", argv[0]);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        usage(argv);
    }

    yyin = fopen(argv[1], "r");

    if (!yyin)
    {
        printf("Could not open %s\n", argv[1]);
        exit(1);
    }

    while (1)
    {
        token_t t = yylex();

        if (t == TOKEN_EOF)
        {
            printf("End of file\n");
            break;
        }

        printf("Token: %s value: %s\n", XSTR(t), yytext);
    }

    return 0;
}