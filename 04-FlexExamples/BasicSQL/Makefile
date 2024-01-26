CC = gcc
FLEX = flex

all: scanner

scanner: scanner.o main.o
	$(CC) -v scanner.o main.o -o $@

scanner.o: scanner.c
	$(CC) -c $< -o $@

scanner.c: scanner.flex
	$(FLEX) -o $@ $<

main.o: main.c
	$(CC) -c $< -o $@

.PHONY:
clean:
	$(RM) scanner scanner.c *.o