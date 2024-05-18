CXX = g++
FLEX = flex
BISON = bison -Wcounterexamples --defines=token.h

all: interpreter

interpreter: parser.o scanner.o main.o
	$(CXX) scanner.o parser.o main.o -o interpreter

parser.o: parser.c
	$(CXX) -c parser.c

parser.c: parser.bison
	$(BISON) -v --output parser.c parser.bison

scanner.o: token.h scanner.c
	$(CXX) -c scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.c
	$(CXX) -c main.c

.PHONY:
clean:
	$(RM) *.o parser.c parser.output token.h scanner.c interpreter