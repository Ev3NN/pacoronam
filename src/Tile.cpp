#include "Tile.hpp"

void Tile::init_variables(c_TileType tileType, c_uint rows, c_uint cols) {
	this->tileType = tileType;
	this->rows = rows;
	this->cols = cols;
}

Tile::Tile() {	
	init_variables();
}

Tile::Tile(c_TileType& tileType, c_uint& rows, c_uint& cols) {
	init_variables(tileType, rows, cols);
}

sf::Vector2f Tile::get_tile_centre() const {

	float centreX = (cols + 0.5f) * pxSize;
	float centreY = (rows + 0.5f) * pxSize;

	return sf::Vector2f(centreX, centreY);
}