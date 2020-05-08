#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
	protected:
		TileType tileType;
		uint pxSize;
		float posX, posY;		

	public:
		Tile();
		Tile(TileType tileType);
};

#endif // !TILE_HPP