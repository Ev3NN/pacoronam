#include "FoodTile.hpp"

void FoodTile::init_shape() {

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

FoodTile::FoodTile(c_TileType& tileType, c_uint& rows, c_uint& cols) : Tile(tileType, rows, cols) {
	init_shape();
}

void FoodTile::render(sf::RenderTarget* target) {
	target->draw(shape);
}