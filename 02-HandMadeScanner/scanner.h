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
    int c = fgetc(f);
    
    while (isspace(c))
    {
        c = fgetc(f);
    }

    ungetc(c, f);
}

BOOL is_next_equal_char(FILE* f)
{
    int c = fgetc(f);

    if (c == '=')
    {
        return TRUE;
    }

    ungetc(c, f);

    return FALSE;
}

Token build_integer_token(FILE* f, char c)
{
    int n = 0;
    Token t;
    t.token = TOKEN_INTEGER;
    t.value[n++] = c;

    c = fgetc(f);

    while (isdigit(c))
    {
        t.value[n++] = c;
        c = fgetc(f);        
    }

    t.value[n] = '\0';
    ungetc(c, f);

    return t;
}

Token build_identifier_token(FILE* f, char c)
{
    int n = 0;
    Token t;
    t.token = TOKEN_IDENTIFIER;
    t.value[n++] = c;

    c = fgetc(f);
    
    if (t.value[0] == '_' && (c == '_' || !isalnum(c)))
    {
        while (c == '_')
        {
            t.value[n++] = c;
            c = fgetc(f);
        }

        t.token = TOKEN_UNKNOWN;
        t.value[n--] = '\0';

        if (isalnum(c))
        {
            ungetc(c, f);
            ungetc('_', f);
            t.value[n--] = '\0';
        }
        else
        {
            ungetc(c, f);
        }

        return t;
    }

    while (c == '_' || isalnum(c))
    {
        t.value[n++] = c;
        c = fgetc(f);
    }

    t.value[n] = '\0';
    ungetc(c, f);

    return t;
}

Token scan_token(FILE* f)
{
    Token result;

    skip_whitespaces(f);

    if (feof(f))
    {
        result.value[0] = '\0';
        result.token = TOKEN_EOF;
        return result;
    }

    int c = fgetc(f);

    if (c == '*')
    {
        result.token = TOKEN_MULTIPLY;
        result.value[0] = '*';
        result.value[1] = '\0';
        return result;
    }
    
    if (c == '=')
    {
        result.value[0] = '=';
        if (is_next_equal_char(f))
        {
            result.token = TOKEN_EQUAL;
            result.value[1] = '=';
            result.value[2] = '\0';
            return result;
        }
        result.token = TOKEN_ASSIGN;
        result.value[1] = '\0';
        return result;
    }
    
    if (c == '!')
    {
        result.value[0] = '!';
        if (is_next_equal_char(f))
        {
            result.token = TOKEN_NOT_EQUAL;
            result.value[1] = '=';
            result.value[2] = '\0';
            return result;
        }
        result.token = TOKEN_NOT;
        result.value[1] = '\0';
        return result;
    }

    if (isdigit(c))
    {
        return build_integer_token(f, c);
    }

    if (c == '_' || isalpha(c))
    {
        return build_identifier_token(f, c);
    }

    result.value[0] = c;
    result.value[1] = '\0';
    result.token = TOKEN_UNKNOWN;
    return result;
}