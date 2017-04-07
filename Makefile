CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2

isomorphism: main.o graph.o
	$(CXX) main.o graph.o -o isomorphism $(CXXFLAGS)

test: main_test.o graph.o
	$(CXX) main_test.o graph.o -o isomorphism_test $(CXXFLAGS)

main.o: main.cpp graph.h
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)

graph.o: graph.cpp graph.h
	$(CXX) -c graph.cpp -o graph.o $(CXXFLAGS)

main_test.o: main.cpp graph.h
	$(CXX) -c main.cpp -o main_test.o $(CXXFLAGS) -DGRAPH_TEST

clean:
	rm -f *.o