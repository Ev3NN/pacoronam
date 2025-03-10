#include <iostream>

#include "Tile.hpp"

// static data member initialisation
bool Tile::isInfected = false;
uint Tile::frameCount = 0;
uint Tile::secCount = 0;

/* --- PROTECTED FUNCTIONS --- */

void Tile::init_variables(c_TileType tileType, c_uint rows, c_uint cols) {
	this->tileType = tileType;
	this->rows = rows;
	this->cols = cols;
	this->isInfected = false;
	this->frameCount = 0;
	this->secCount = 0;
}

void Tile::init_food() {
	if(tileType == TREAT_TILE || tileType == PILL_TILE)
		food = std::make_shared<Food>(tileType, rows, cols);
	else
		food = nullptr;
}

sf::FloatRect Tile::get_bounds() {
	return sf::FloatRect(sf::Vector2f(cols * CELL_SIZE, rows * CELL_SIZE), 
							 sf::Vector2f(CELL_SIZE, CELL_SIZE));
}

bool Tile::is_out_of_bounds() {
	return rows < 0 || rows > (int) GRID_ROWS || cols < 0 || cols > (int) GRID_COLS;
}

bool Tile::compare(Tile* nextTile) {
	if(!nextTile)
		return false;

	// No need to compare tiletype ?
	if(this->rows == nextTile->rows && this->cols == nextTile->cols)
		return true;

	return false;
}

bool Tile::is_restricted_area(bool canEnterMonsterHouse) {
	return (tileType >= 1 && tileType <= 13) || (tileType == DOOR_TILE && !canEnterMonsterHouse);
}

void Tile::reset(c_uint& i) {
	// Detects pills by hardcoding their position in the vector
	if(i == 30 || i == 35 || i == 158 || i == 177)
		tileType = PILL_TILE;
	else
		tileType = TREAT_TILE;
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


// Tile::Tile(const Tile& other): Tile(other.tileType, other.rows, other.cols) {

// 	this->food = std::make_shared<Food>(*(other.food));
// }

Tile::Tile(const std::shared_ptr<Tile>& other) : Tile(other->tileType, other->rows, other->cols) {

	this->food = other->food;
}

// Tile& Tile::operator=(Tile other) {
	
// 	this->tileType = other.tileType;
// 	this->rows = other.rows;
// 	this->cols = other.cols;
// 	this->food = std::make_shared<Food>(*(other.food));
// }

const sf::Vector2f Tile::get_tile_centre() const {

	c_double centreX = (cols + 0.5f) * pxSize;
	c_double centreY = (rows + 0.5f) * pxSize;

	return sf::Vector2f(centreX, centreY);
}

void Tile::render(sf::RenderTarget* target) {
	food->render(target);
}

void Tile::get_infected() {
	isInfected = true;
	frameCount = FPS;
	secCount = 6;
}

bool Tile::get_is_infected() {
	return isInfected;
}

void Tile::update_timer() {
	if(!isInfected)
		return;

	--frameCount;

	if(!frameCount) {
		frameCount = FPS;
		--secCount;
		if(!secCount){
			isInfected = false;
		}
	}
	
}
