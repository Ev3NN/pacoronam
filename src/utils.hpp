#ifndef UTILS_HPP
#define UTILS_HPP

#include "constants.hpp"

// Returns true if the coordinates (i, j) is valid inside the grid.
// Returns false otherwise
bool out_of_grid(c_int& i, c_int& j);

// Returns true if the coordinates indicates the sentinel value for the tunnel
bool in_tunnel(c_int& i, c_int& j);

bool cmp_float(c_float& a, c_float& b, float epsilon = 0.005f);

#endif // !UTILS_HPP