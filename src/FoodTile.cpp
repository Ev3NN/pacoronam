#include "FoodTile.hpp"
#include <iostream>
#include <fstream>

FoodTile::FoodTile() : isInfected(false) {}

FoodTile::FoodTile(TileType tileType, uint height, uint width) : Tile(tileType), isInfected(false) {

	if(tileType == TREAT_TILE)
		shape = sf::CircleShape(CELL_SIZE / 6);
	// tileType == PILL_SIZE
	else
		shape = sf::CircleShape(CELL_SIZE / 3);

	shape.setFillColor(sf::Color(247, 192, 158));
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition((width + 0.5) * CELL_SIZE, (height + 0.5) * CELL_SIZE);
}

bool FoodTile::is_infected() {
	std::cout << "Am I infected ?\n";
	return false;
}