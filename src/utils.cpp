#include <cmath>

#include "utils.hpp"

double distance(sf::Vector2f p1, sf::Vector2f p2) {

	double x = pow((p1.x - p2.x), 2);
	double y = pow((p1.y - p2.y), 2);
	
	return sqrt(x + y);
}

bool is_out_of_bounds(c_int& rows, c_int& cols) {
	return rows < 0 || rows >= (int) GRID_ROWS || cols < 0 || cols >= (int) GRID_COLS;
}