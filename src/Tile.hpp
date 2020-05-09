#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
	protected:
		static c_uint pxSize = CELL_SIZE;

		TileType tileType;
		uint rows, cols;

		void init_variables(c_TileType tileType = NO_TILE, c_uint rows = 0, c_uint cols = 0);

	public:

		sf::Vector2f get_tile_centre() const;

		Tile();
		Tile(c_TileType& tileType, c_uint& rows, c_uint& cols);
};

#endif // !TILE_HPP