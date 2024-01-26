#pragma once

typedef enum
{
    TOKEN_EOF = 0,
    TOKEN_IF = 258,
    TOKEN_THEN = 259,
    TOKEN_ELSE = 260,
    TOKEN_EQUAL = 261,
    TOKEN_LESS = 262,
    TOKEN_LESS_EQ = 263,
    TOKEN_GREATER = 264,
    TOKEN_GREATER_EQ = 265,
    TOKEN_NOT_EQUAL = 266,
    TOKEN_AND = 267,
    TOKEN_OR = 268,
    TOKEN_NOT = 269,
    TOKEN_NEG = 270,
    TOKEN_INT = 271,
    TOKEN_IDENTIFIER = 272
}
token_t;

inline const char* to_str(token_t t)
{
    switch (t)
    {
        case TOKEN_IF: return "IF";
        case TOKEN_THEN: return "THEN";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_LESS_EQ: return "LESS_EQ";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQ: return "GREATER_EQ";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_NEG: return "NEG";
        case TOKEN_INT: return "INT";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
    }
}