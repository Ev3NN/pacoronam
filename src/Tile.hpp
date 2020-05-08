#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
	protected:
		TileType tileType;
		uint rows, cols;
		uint pxSize;

	public:

		sf::Vector2f get_tile_centre();

		Tile();
		Tile(TileType tileType, uint rows, uint cols);
};

#endif // !TILE_HPP