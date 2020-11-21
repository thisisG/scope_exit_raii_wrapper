CXX=clang++
CXXFLAGS=-std=c++17 -I.
DEPS = scope_exit.hpp scope_exit_tests.hpp
OBJ = scope_exit_tests.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

scope_exit_tests: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	./scope_exit_tests

.PHONY: clean

clean:
	rm *.o scope_exit_tests
