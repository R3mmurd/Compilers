CC = clang

all: parser

parser: token.h scanner.h parser.h parser.c
	$(CC) $@.c -o $@

.PHONY:
clean:
	$(RM) parser