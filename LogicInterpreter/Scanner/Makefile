CC = gcc
FLEX = flex

all: lexer

lexer: scanner.o main.o
	$(CC) -v scanner.o main.o -o lexer

scanner.o: token.h scanner.c
	$(CC) -c scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.c
	$(CC) -c main.c

.PHONY:
clean:
	$(RM) *.o scanner.c lexer