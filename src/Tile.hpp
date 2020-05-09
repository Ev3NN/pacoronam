#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
	protected:
		static c_uint pxSize = CELL_SIZE;
		uint rows, cols;

		TileType tileType;
		

		void init_variables(c_TileType tileType = NO_TILE, c_uint rows = 0, c_uint cols = 0);

		sf::FloatRect get_bounds();

	public:
		

		const sf::Vector2f get_tile_centre() const;

		Tile();
		Tile(c_TileType& tileType, c_uint& rows, c_uint& cols);

	friend class Character;
	friend class Grid;
};

#endif // !TILE_HPP