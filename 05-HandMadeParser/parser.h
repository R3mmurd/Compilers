/*
    Helper functions to parse the following grammar by using the
    recursive descent parsing

    P  -> E
    E  -> T E'
    E' -> + T E'
    E' -> epsilon
    T  -> F T'
    T' -> * F T'
    T' -> epsilon
    F  -> (E)
    F  -> int
*/

#pragma once

#include "scanner.h"

BOOL parse_p(FILE*);
BOOL parse_e(FILE*);
BOOL parse_e_prime(FILE*);
BOOL parse_t(FILE*);
BOOL parse_t_prime(FILE*);
BOOL parse_f(FILE*);

BOOL parse_p(FILE* f)
{
    return parse_e(f) && expect_token(f, TOKEN_EOF);
}

BOOL parse_e(FILE* f)
{
    return parse_t(f) && parse_e_prime(f);
}

BOOL parse_e_prime(FILE* f)
{
    Token t = scan_token(f);

    if (t.token == TOKEN_PLUS)
    {
        return parse_t(f) && parse_e_prime(f);
    }

    put_token_back(f, t);
    return TRUE;
}

BOOL parse_t(FILE* f)
{
    return parse_f(f) && parse_t_prime(f);
}

BOOL parse_t_prime(FILE* f)
{
    Token t = scan_token(f);

    if (t.token == TOKEN_MULTIPLY)
    {
        return parse_f(f) && parse_t_prime(f);
    }

    put_token_back(f, t);
    return TRUE;
}

BOOL parse_f(FILE* f)
{
    Token t = scan_token(f);

    if (t.token == TOKEN_LPAREN)
    {
        return parse_e(f) && expect_token(f, TOKEN_RPAREN);
    }
    else if (t.token == TOKEN_INT)
    {
        return TRUE;
    }

    printf("Parse error: unexpected token %s\n", to_str(t.token));
    return FALSE;
}