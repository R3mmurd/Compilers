CXX = clang++ -std=c++17
CC = clang
OBJ = ast_node_interface.o datatype.o declaration.o expression.o statement.o

default: demo_c_function

all: example demo_c_function

example:
	$(CC) $@.c -o $@

demo_c_function: $(OBJ) demo_c_function.cpp
	$(CXX) -I. $@.cpp -o $@ $(OBJ)

ast_node_interface.o: ast_node_interface.cpp ast_node_interface.hpp 
	$(CXX) -I. -c $< -o $@

datatype.o: datatype.cpp datatype.hpp 
	$(CXX) -I. -c $< -o $@

declaration.o: declaration.cpp declaration.hpp 
	$(CXX) -I. -c $< -o $@

expression.o: expression.cpp expression.hpp 
	$(CXX) -I. -c $< -o $@

statement.o: statement.cpp statement.hpp 
	$(CXX) -I. -c $< -o $@

.PHONY:
clean:
	$(RM) $(OBJ) example demo_c_function
