#include <iostream>
#include <algorithm>
#include <set>
#include <functional>
#include <stdexcept>
#include <string>
#include <sstream>
#include "graph.h"

Graph::Graph(std::istream& is)
	: vertices(0)
	, edges(0)
{
	std::string line, rest;
	std::stringstream ss;
	getline(is, line);
	ss.str(line);
	if (!(ss >> vertices) || ss >> rest) {
		throw std::runtime_error("parsing error in line: " + line);
	}

	matrix.resize(vertices);
	degrees.resize(vertices);
	cycles.resize(vertices);

	int from, to;
	while (getline(is, line)) {
		ss.clear();
		ss.str(line);
		if (!(ss >> from >> to) || ss >> rest) {
			throw std::runtime_error("parsing error in line: " + line);
		}
		add_edge(from, to);
	}

	count_cycles();
}

void Graph::add_edge(int from, int to)
{
	if (from >= vertices || to >= vertices) {
		throw std::logic_error("incorrect edge: " + std::to_string(from) + " " + std::to_string(to));
	}

	++edges;
	matrix(from, to) = 1;
	matrix(to, from) = 1;
	++degrees[from];
	if (from != to) {
		++degrees[to];
	}
}

void Graph::count_cycles()
{
	Matrix<int> cubed(matrix);
	for (int i = 0; i < vertices; ++i) {
		cubed(i, i) = 0;
	}
	cubed = cubed * cubed * cubed;
	for (int i = 0; i < vertices; ++i) {
		cycles[i] = cubed(i, i);
	}
}

bool Graph::isomorphic(const Graph& other, std::vector<int>& mapping) const
{
	//compare number of vertices and edges
	if (vertices != other.vertices || edges != other.edges) {
		return false;
	}

	//check sequence of degrees
	if (!std::is_permutation(degrees.begin(), degrees.end(), other.degrees.begin())) {
		return false;
	}

	//check sequence of cycles
	if (!std::is_permutation(cycles.begin(), cycles.end(), other.cycles.begin())) {
		return false;
	}

	//check all possible mappings
	std::set<int> unique_degrees(degrees.begin(), degrees.end());
	std::set<int> unique_cycles(cycles.begin(), cycles.end());
	std::vector<int> candidates(vertices);
	std::vector<std::vector<int>::iterator> sentinels;
	mapping.resize(vertices);

	auto iter = candidates.begin();
	auto index = 0;
	for (auto degree : unique_degrees) {
		for (auto cycle : unique_cycles) {
			for (int vertex = 0; vertex < vertices; ++vertex) {
				if (degree == other.degrees[vertex] && cycle == other.cycles[vertex]) {
					*iter++ = vertex;
				}
				if (degree == degrees[vertex] && cycle == cycles[vertex]) {
					mapping[vertex] = index++;
				}
			}
			if ((sentinels.empty() && iter != candidates.begin()) || (sentinels.size() && sentinels.back() != iter)) {
				sentinels.push_back(iter);
			}
		}
	}

	auto translate = [&](int index)->int {
		return candidates[mapping[index]];
	};

	auto compare = [&]()->bool {
		for (int row = 0; row < vertices; ++row) {
			for (int col = row; col < vertices; ++col) {
				if (matrix(row, col) != other.matrix(translate(row), translate(col))) {
					return false;
				}
			}
		}
		return true;
	};

	auto next_permutation = [&]()->bool {
		auto first = candidates.begin();
		for (auto last : sentinels) {
			if (std::next_permutation(first, last)) {
				return true;
			}
			first = last;
		}
		return false;
	};

	do {
		if (compare()) {
			for (int vertex = 0; vertex < vertices; ++vertex) {
				mapping[vertex] = translate(vertex);
			}
			return true;
		}
	} while (next_permutation());
	return false;
}