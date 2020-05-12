#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"

// Returns true if the coordinates (i, j) is valid inside the grid.
// Returns false otherwise
bool out_of_grid(c_int& i, c_int& j);

// Returns true if the coordinates indicates the sentinel value for the tunnel
bool is_tunnel(c_int& i, c_int& j);

bool cmp_float(c_double& a, c_double& b, float epsilon = 0.005f);

bool cmp_point(sf::Vector2f a, sf::Vector2f b, c_float& epsilon);

template <class T>
void swap(T& x, T& y);

#endif // !UTILS_HPP