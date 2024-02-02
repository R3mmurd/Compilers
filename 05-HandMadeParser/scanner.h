/*
    Helper functions for a scanner that tokenize the input stream
    to support operations to add and multiply integer literals.
*/

#pragma once

#include <ctype.h>
#include <stdio.h>

#include "token.h"

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void skip_whitespaces(FILE* f)
{
    char c = fgetc(f);
    
    while (isspace(c))
    {
        c = fgetc(f);
    }

    ungetc(c, f);
}

BOOL is_next_equal_char(FILE* f)
{
    char c = fgetc(f);

    if (c == '=')
    {
        return TRUE;
    }

    ungetc(c, f);

    return FALSE;
}

Token build_integer_token(FILE* f, char c)
{
    Token t = make_token();
    t.token = TOKEN_INT;
    t.value[t.size++] = c;

    c = fgetc(f);

    while (isdigit(c))
    {
        t.value[t.size++] = c;
        c = fgetc(f);        
    }

    t.value[t.size++] = '\0';
    ungetc(c, f);

    return t;
}

Token scan_token(FILE* f)
{
    Token result = make_token();

    skip_whitespaces(f);

    if (feof(f))
    {
        result.value[result.size++] = '\0';
        result.token = TOKEN_EOF;
        return result;
    }

    char c = fgetc(f);

    if (c == '(')
    {
        result.token = TOKEN_LPAREN;
        result.value[result.size++] = '(';
        result.value[result.size++] = '\0';
        return result;
    }

    if (c == ')')
    {
        result.token = TOKEN_RPAREN;
        result.value[result.size++] = ')';
        result.value[result.size++] = '\0';
        return result;
    }

    if (c == '+')
    {
        result.token = TOKEN_PLUS;
        result.value[result.size++] = '+';
        result.value[result.size++] = '\0';
        return result;
    }

    if (c == '*')
    {
        result.token = TOKEN_MULTIPLY;
        result.value[result.size++] = '*';
        result.value[result.size++] = '\0';
        return result;
    }
    
    if (isdigit(c))
    {
        return build_integer_token(f, c);
    }

    result.value[result.size++] = c;
    result.value[result.size++] = '\0';
    result.token = TOKEN_UNKNOWN;
    return result;
}

void put_token_back(FILE* f, Token t)
{
    for (int i = t.size - 2; i >= 0; --i)
    {
        ungetc(t.value[i], f);
    }
}

BOOL expect_token(FILE* f, token_t expected_token_type)
{
    Token t = scan_token(f);

    if (t.token == expected_token_type)
    {
        return TRUE;
    }

    put_token_back(f, t);

    return FALSE;
}