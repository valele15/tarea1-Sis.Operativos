CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lpthread

all: planificador

planificador: main.o dag.o executor.o
	$(CXX) $(CXXFLAGS) -o planificador main.o dag.o executor.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

dag.o: dag.cpp dag.hpp
	$(CXX) $(CXXFLAGS) -c dag.cpp

executor.o: executor.cpp executor.hpp
	$(CXX) $(CXXFLAGS) -c executor.cpp

clean:
	rm -f *.o planificador
