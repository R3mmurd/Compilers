CXX = clang++ -std=c++17
INCLUDES = -I.

all: DFA_demo

DFA_demo: DFA.hpp DFA_demo.cpp
	$(CXX) $(INCLUDES) $@.cpp -o $@

.PHONY:
clean:
	$(RM) DFA_demo