#ifndef FOOD_TILE_HPP
#define FOOD_TILE_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"

// Derived class representing a tile where there was a treat or a power pill initially
class Food {
	private:
		/* --- PRIVATE DATA MEMBERS --- */

		// Visual appearance
		sf::CircleShape shape;

		// Add an 'eaten' data member so we do not need to hardcode
		// index of PILL_TILE while resetting

		/* --- PRIVATE FUNCTIONS --- */

		// Initialises the shape
		void init_shape(c_TileType& tileType, c_uint& rows, c_uint& cols);

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructor
		Food(c_TileType& tileType, c_uint& rows, c_uint& cols);

		// Draws the needed data members on the window
		void render(sf::RenderTarget* target);
};

#endif // !FOOD_TILE_HPP