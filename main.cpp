//#define GRAPH_TEST

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include "graph.h"

#ifdef GRAPH_TEST
#include <random>
#include <algorithm>
#include <numeric>
#define MIN_VERTICES 8
#define MAX_VERTICES 14
#define LOOPS 1000
#define WIDTH 40

bool perform_test()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> n_vertices(MIN_VERTICES, MAX_VERTICES);
	int vertices = n_vertices(rng);
	std::uniform_int_distribution<int> n_edges(0, (vertices * (vertices - 1) / 2));
	int edges = n_edges(rng);

	std::vector<int> mapping(vertices);
	std::iota(mapping.begin(), mapping.end(), 0);
	std::shuffle(mapping.begin(), mapping.end(), rng);

	std::stringstream input1, input2;
	input1 << vertices << std::endl;
	input2 << vertices << std::endl;

	std::uniform_int_distribution<int> vertex(0, vertices - 1);
	for (int i = 0; i < edges; ++i) {
		int from, to;
		from = vertex(rng);
		to = vertex(rng);
		input1 << from << " " << to << std::endl;
		input2 << mapping[from] << " " << mapping[to] << std::endl;
	}

	Graph graph1(input1);
	Graph graph2(input2);

	return (graph1.isomorphic(graph2, mapping));
}
#endif

int main(int argc, char** argv)
{
#ifdef GRAPH_TEST
	std::cout << "RUNNING GRAPHS TESTS...\n";
	int passed = 0;
	for (int i = 1; i <= LOOPS; ++i) {
		std::cout << "[";
		int arrow = WIDTH * i / LOOPS;
		for (int pos = 1; pos <= WIDTH; ++pos) {
			if (pos < arrow) {
				std::cout << "=";
			}
			else if (pos == arrow) {
				std::cout << ">";
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << "] " << i << "/" << LOOPS;
		if (perform_test()) {
			++passed;
		}
		std::cout << "\r";
		std::cout.flush();
	}
	std::cout << "\nPASSED: " << passed << "/" << LOOPS << std::endl;

#else
	try {
		if (argc != 3) {
			throw std::invalid_argument("usage: " + std::string(argv[0]) + " [graph1_file] [graph2_file]");
		}

		std::ifstream input;
		input.open(argv[1]);
		if (input.fail()) {
			throw std::runtime_error("cannot open file " + std::string(argv[1]));
		}
		Graph graph1(input);
		input.close();

		input.open(argv[2]);
		if (input.fail()) {
			throw std::runtime_error("cannot open file " + std::string(argv[2]));
		}
		Graph graph2(input);
		input.close();

		std::vector<int> mapping;
		std::chrono::high_resolution_clock::time_point begin, end;
		begin = std::chrono::high_resolution_clock::now();
		if (graph1.isomorphic(graph2, mapping)) {
			std::cout << "isomorphic\n";
			for (std::size_t i = 0; i < mapping.size(); ++i) {
				std::cout << i << " --> " << mapping[i] << "\n";
			}
		}
		else {
			std::cout << "non-isomorphic\n";
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << std::endl << std::chrono::duration<double, std::milli>(end - begin).count() << "ms\n";
	}
	//catch potential errors
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

#endif
	return 0;
}