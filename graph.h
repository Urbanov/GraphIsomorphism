#pragma once

#include <vector>
#include <iostream>
#include "matrix.h"

class Graph {
private:
	int vertices;
	int edges;
	Matrix<int> matrix;
	std::vector<int> degrees;
	std::vector<int> cycles;
	void add_edge(int from, int to);
	void count_cycles();
	
public:
	Graph() = delete;
	explicit Graph(std::istream& is);
	bool isomorphic(const Graph& other, std::vector<int>& mapping) const;
};