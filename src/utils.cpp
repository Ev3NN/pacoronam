#include <cmath>

#include "utils.hpp"

bool out_of_grid(c_int& i, c_int& j) {
	// Avoids signed-unsigned comparison
	c_int rows = GRID_ROWS;
	c_int cols = GRID_COLS;

	if(i >= rows || i < 0 || j >= cols || j < 0)
		return true;

	return false;
}

bool is_tunnel(c_int& i, c_int& j) {

	// Avoids signed-unsigned comparison
	c_int cols = GRID_COLS;

	return (i == 17 && j == -1) || (i == 17 && j == cols);
}

bool cmp_float(c_double& a, c_double& b, float epsilon) {
	return (std::fabs(a - b) < epsilon);
}

bool cmp_point(sf::Vector2f a, sf::Vector2f b, c_float& epsilon) {
	return cmp_float(a.x, b.x, epsilon) && cmp_float(a.y, b.y, epsilon);
}

template <class T>
void swap(T& x, T& y) {
	T tmp = y;
	y = x;
	x = tmp;
}