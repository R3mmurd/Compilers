CC = clang

all: scanner

scanner: token.h scanner.h scanner.c
	$(CC) $@.c -o $@

.PHONY:
clean:
	$(RM) scanner