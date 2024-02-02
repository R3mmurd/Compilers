#include "parser.h"

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

    if (parse_p(in))
    {
        printf("Parse successful\n");
    }
    else
    {
        printf("Parse failed\n");
    }

    return 0;
}