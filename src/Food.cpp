#include "Food.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Food::init_shape(c_TileType& tileType, c_uint& rows, c_uint& cols) {
	if(tileType == TREAT_TILE)
		shape = sf::CircleShape(CELL_SIZE / 6);
	// tileType == PILL_SIZE
	else
		shape = sf::CircleShape(CELL_SIZE / 3);

	shape.setFillColor(sf::Color(247, 192, 158));
	shape.setOrigin(shape.getRadius(), shape.getRadius());

	float centreX = (cols + 0.5f) * CELL_SIZE;
	float centreY = (rows + 0.5f) * CELL_SIZE;

	shape.setPosition(centreX, centreY);
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Food::Food(c_TileType& tileType, c_uint& rows, c_uint& cols) {
	init_shape(tileType, rows, cols);
}

void Food::render(sf::RenderTarget* target) {
	target->draw(shape);
}