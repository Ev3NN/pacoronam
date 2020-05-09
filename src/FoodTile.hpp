#ifndef FOOD_TILE_HPP
#define FOOD_TILE_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Tile.hpp"

class FoodTile : public Tile {
	private:
		sf::CircleShape shape;

		// Private functions
		void init_shape();

	public:
		// Constructor
		FoodTile(c_TileType& tileType, c_uint& rows, c_uint& cols);

		// Public functions
		void render(sf::RenderTarget* target);
};

#endif // !FOOD_TILE_HPP