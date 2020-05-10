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

bool in_tunnel(c_int& i, c_int& j) {

	// Avoids signed-unsigned comparison
	c_int cols = GRID_COLS;

	return (i == 17 && j == -1) || (i == 17 && j == cols);
}

bool cmp_float(c_float& a, c_float& b, float epsilon) {
	return (std::fabs(a - b) < epsilon);
}