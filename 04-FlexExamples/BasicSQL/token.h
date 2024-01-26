#pragma once

typedef enum
{
    TOKEN_EOF = 0,
    TOKEN_SELECT = 258,
    TOKEN_FROM = 259,
    TOKEN_WHERE = 260,
    TOKEN_COMMA = 261,
    TOKEN_SEMICOLON = 262,
    TOKEN_WILDCARD = 263,
    TOKEN_IDENTIFIER = 264
}
token_t;

inline const char* to_str(token_t t)
{
    switch (t)
    {
        case TOKEN_SELECT: return "SELECT";
        case TOKEN_FROM: return "FROM";
        case TOKEN_WHERE: return "WHERE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_WILDCARD: return "WILDCARD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
    }
}