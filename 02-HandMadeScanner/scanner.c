/*
    A handmade scanner to tokenize a little language that supports
    operators such as multiply, logic negation, assignment, equal, not equal,
    integer values, and identifiers that begin with underscore or an alpha
    character followed by any alphanumeric characters or any underscores.
*/

#include "scanner.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE* in = fopen(argv[1], "r");

    if (!in)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    while (TRUE)
    {
        Token t = scan_token(in);

        if (t.token == TOKEN_EOF)
        {
            break;
        }

        printf("Token: %s value: %s\n", to_str(t.token), t.value);
    }

    return 0;
}