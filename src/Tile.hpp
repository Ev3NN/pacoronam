#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
	public:
		Tile();
		Tile(TileType tileType);

		TileType tileType;
		c_uint size = CELL_SIZE;

		Tile& operator = (const Tile& other);
};

#endif