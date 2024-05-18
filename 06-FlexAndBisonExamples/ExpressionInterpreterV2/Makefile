CXX = g++
FLEX = flex
BISON = bison -Wcounterexamples --defines=token.h

all: interpreter

interpreter: parser.o scanner.o main.o expression.o
	$(CXX) scanner.o parser.o main.o expression.o -o interpreter

parser.o: parser.c
	$(CXX) -c -I. parser.c

parser.c: parser.bison
	$(BISON) -v --output parser.c parser.bison

scanner.o: token.h scanner.c
	$(CXX) -c scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.c
	$(CXX) -c -I. main.c

expression.o: expression.hpp expression.cpp
	$(CXX) -c -I. expression.cpp

.PHONY:
clean:
	$(RM) *.o parser.c parser.output token.h scanner.c interpreter