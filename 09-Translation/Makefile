CXX = g++ -std=c++17 -O0 -g
OBJ = ast_node_interface.o datatype.o declaration.o expression.o statement.o symbol_table.o

default: demo_program

all: demo_program

demo_program: $(OBJ) demo_program.cpp
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

symbol_table.o: symbol_table.cpp symbol_table.hpp 
	$(CXX) -I. -c $< -o $@

.PHONY:
clean:
	$(RM) $(OBJ) demo_program demo_program.py
