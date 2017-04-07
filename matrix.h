#pragma once

#include <vector>

template <typename Type>
class Matrix {
private:
	std::vector<std::vector<Type>> matrix;

public:
	Matrix() = default;

	void resize(std::size_t size)
	{
		matrix.resize(size);
		for (auto& row : matrix) {
			row.resize(size);
		}
	}

	Type& operator()(std::size_t row, std::size_t col)
	{
		return matrix[row][col];
	}

	const Type& operator()(std::size_t row, std::size_t col) const
	{
		return matrix[row][col];
	}

	Matrix operator*(const Matrix& other) const
	{
		Matrix product;
		product.resize(matrix.size());
		for (std::size_t row = 0; row < matrix.size(); ++row) {
			for (std::size_t col = 0; col < matrix.size(); ++col) {
				for (std::size_t inner = 0; inner < matrix.size(); ++inner) {
					product.matrix[row][col] += matrix[row][inner] * other.matrix[inner][col];
				}
			}
		}
		return product;
	}
};