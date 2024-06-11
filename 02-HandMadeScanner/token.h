#pragma once

typedef enum
{
    TOKEN_EOF,
    TOKEN_MULTIPLY,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_NOT,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_UNKNOWN
}
token_t;

#define MAX_BUFFER_LENGTH 1024

typedef char buffer_t[MAX_BUFFER_LENGTH];

typedef struct
{
    token_t  token;
    buffer_t value;
}
Token;

const char* to_str(token_t t)
{
    switch(t)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_NOT: return "NOT";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_UNKNOWN: return "UNKNOWN";
    }
}