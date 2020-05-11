#include "Tile.hpp"

/* --- PROTECTED FUNCTIONS --- */

void Tile::init_variables(c_TileType tileType, c_uint rows, c_uint cols) {
	this->tileType = tileType;
	this->rows = rows;
	this->cols = cols;
}

void Tile::init_food() {
	if(tileType == TREAT_TILE || tileType == PILL_TILE)
		food = new Food(tileType, rows, cols);
	else
		food = nullptr;
}

sf::FloatRect Tile::get_bounds() {
	return sf::FloatRect(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE), 
							 sf::Vector2f(CELL_SIZE, CELL_SIZE));
}

bool Tile::compare(Tile* nextTile) {
	if(!nextTile)
		return false;

	// No need to compare tiletype ?
	if(this->rows == nextTile->rows && this->cols == nextTile->cols)
		return true;

	return false;
}

bool Tile::is_wall() {
	return tileType >= 1 && tileType <= 13;
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Tile::Tile() {	
	init_variables();
	init_food();
}

Tile::Tile(c_TileType& tileType, c_uint& rows, c_uint& cols) {
	init_variables(tileType, rows, cols);
	init_food();
}

Tile::~Tile() {
	if(food) {
		delete food;
	}
}

const sf::Vector2f Tile::get_tile_centre() const {

	c_float centreX = (cols + 0.5f) * pxSize;
	c_float centreY = (rows + 0.5f) * pxSize;

	return sf::Vector2f(centreX, centreY);
}

void Tile::render(sf::RenderTarget* target) {
	if(food)
		food->render(target);
}