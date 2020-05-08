#include "FoodTile.hpp"

void FoodTile::init_shape() {

	if(tileType == TREAT_TILE)
		shape = sf::CircleShape(CELL_SIZE / 6);
	// tileType == PILL_SIZE
	else
		shape = sf::CircleShape(CELL_SIZE / 3);

	shape.setFillColor(sf::Color(247, 192, 158));
	shape.setOrigin(shape.getRadius(), shape.getRadius());

	float centreX = (cols + 0.5) * CELL_SIZE;
	float centreY = (rows + 0.5) * CELL_SIZE;

	shape.setPosition(centreX, centreY);
}

FoodTile::FoodTile() : Tile() {}

FoodTile::FoodTile(TileType tileType, uint rows, uint cols) : Tile(tileType, rows, cols) {
	init_shape();
}

void FoodTile::render(sf::RenderTarget* target) {
	target->draw(shape);
}