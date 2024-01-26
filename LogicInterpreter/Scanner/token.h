#pragma once

enum __token_t
{
  TOKEN_EOF = 0,
  TOKEN_STRING = 258,
  TOKEN_NOT = 259,
  TOKEN_AND = 260,
  TOKEN_OR = 261,
  TOKEN_IMPL = 262,
  TOKEN_BICOND = 263,
  TOKEN_ASSIGN = 264,
  TOKEN_IDENTIFIER = 265,
  TOKEN_LPAREN = 266,
  TOKEN_RPAREN = 267,
  TOKEN_EVAL = 268
};

typedef enum __token_t token_t;

inline const char* token_str(token_t t)
{
  switch(t)
  {
    case TOKEN_EOF: return "<EOF>";
    case TOKEN_STRING: return "<STRING>";
    case TOKEN_NOT: return "<NOT>";
    case TOKEN_AND: return "<AND>";
    case TOKEN_OR: return "<OR>";
    case TOKEN_IMPL: return "<IMPLICATION>";
    case TOKEN_BICOND: return "<BICONDITIONAL>";
    case TOKEN_ASSIGN: return "<ASSIGNMENT>";
    case TOKEN_IDENTIFIER: return "<IDENTIFIER>";
    case TOKEN_LPAREN: return "<L_PAREN>";
    case TOKEN_RPAREN: return "<R_PAREN>";
    case TOKEN_EVAL: return "<EVAL>";
    default: return "<UNKNOWN>";
  }
}