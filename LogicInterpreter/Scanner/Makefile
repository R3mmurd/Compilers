CC = gcc
FLEX = flex

all: scanner

scanner: scanner.o main.o
	$(CC) -v scanner.o main.o -o scanner

scanner.o: token.h scanner.c
	$(CC) -c scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.c
	$(CC) -c main.c

.PHONY:
clean:
	$(RM) *.o scanner.c scanner