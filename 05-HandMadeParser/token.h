#pragma once

typedef enum
{
    TOKEN_EOF,
    TOKEN_PLUS,
    TOKEN_MULTIPLY,
    TOKEN_INT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_UNKNOWN
}
token_t;

#define MAX_BUFFER_LENGTH 1024

typedef char buffer_t[MAX_BUFFER_LENGTH];

typedef struct
{
    int      size;
    token_t  token;
    buffer_t value;
}
Token;

Token make_token()
{
    Token t;
    t.size = 0;
    return t;
}

const char* to_str(token_t t)
{
    switch(t)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_INT: return "INT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_UNKNOWN: return "UNKNOWN";
    }
}