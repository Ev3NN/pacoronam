#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Food.hpp"

// Represents a tile in the grid
class Tile {
	public:
		/* --- PROTECTED DATA MEMBERS --- */

		// Dimensions
		static c_uint pxSize = CELL_SIZE;
		uint rows, cols;

		// Content of the tile
		TileType tileType;

		Food* food;
		
		/* --- PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(c_TileType tileType = NO_TILE, c_uint rows = 0, c_uint cols = 0);

		void init_food();

		// Returns the bounds of the tile
		sf::FloatRect get_bounds();

		// Returns true if the two tiles are the same
		bool compare(Tile* nextTile);

		bool is_restricted_area(bool isMonsterHouseOpen);

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Tile();
		Tile(c_TileType& tileType, c_uint& rows, c_uint& cols);
		~Tile();
		
		// Returns the coordinates of tile's middle point
		const sf::Vector2f get_tile_centre() const;

		// Draws each data member
		void render(sf::RenderTarget* target);
	// Need to refactor this
	
	// Character needs to access theses data members to use 'aboveTile'
};

#endif // !TILE_HPP