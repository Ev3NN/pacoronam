#include "Tile.hpp"

void Tile::init_variables(c_TileType tileType, c_uint rows, c_uint cols) {
	this->tileType = tileType;
	this->rows = rows;
	this->cols = cols;
}

sf::FloatRect Tile::get_bounds() {
	return sf::FloatRect(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE), 
						 sf::Vector2f(CELL_SIZE, CELL_SIZE));
}

Tile::Tile() {	
	init_variables();
}

Tile::Tile(c_TileType& tileType, c_uint& rows, c_uint& cols) {
	init_variables(tileType, rows, cols);
}

const sf::Vector2f Tile::get_tile_centre() const {

	c_float centreX = (cols + 0.5f) * pxSize;
	c_float centreY = (rows + 0.5f) * pxSize;

	return sf::Vector2f(centreX, centreY);
}