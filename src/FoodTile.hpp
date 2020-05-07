#ifndef FOOD_TILE_HPP
#define FOOD_TILE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "Tile.hpp"

class FoodTile : public Tile {

	public:
		FoodTile();
		FoodTile(TileType tileType, uint height, uint width);
		sf::CircleShape shape;

	private:
		bool isInfected;

		bool is_infected();

		

};

#endif