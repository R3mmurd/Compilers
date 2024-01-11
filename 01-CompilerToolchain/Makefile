CXX = g++

all: sample

sample: sample.o
	$(CXX) $< -o $@

sample.o: sample.s
	$(CXX) -c $< -o $@

sample.s: sample_preprocessed.cpp
	$(CXX) -S $< -o $@

sample_preprocessed.cpp: sample.cpp
	$(CXX) -E $< > $@

.PHONY:
clean:
	$(RM) sample_preprocessed.cpp sample.s sample.o sample