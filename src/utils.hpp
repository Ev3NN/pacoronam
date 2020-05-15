#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"

double distance(sf::Vector2f p1, sf::Vector2f p2);

bool is_out_of_bounds(c_int& rows, c_int& cols);

#endif // !UTILS_HPP