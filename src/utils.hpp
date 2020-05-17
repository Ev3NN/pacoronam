#ifndef UTILS_HPP
#define UTILS_HPP


#include "constants.hpp"

double distance(sf::Vector2f p1, sf::Vector2f p2);

bool is_out_of_bounds(c_int& rows, c_int& cols);

bool compare_float(c_float& a, c_float& b, float epsilon = 0.005f);

bool compare_point(sf::Vector2f p1, sf::Vector2f p2, float epsilon = 0.005f);

#endif // !UTILS_HPP