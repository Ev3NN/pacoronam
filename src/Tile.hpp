#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include "constants.hpp"
#include "Food.hpp"

// Represents a tile in the grid
class Tile {
	private:
		/* --- PROTECTED DATA MEMBERS --- */

		// Dimensions
		static c_uint pxSize = CELL_SIZE;
		int rows, cols;

		// Content of the tile
		TileType tileType;

		std::shared_ptr<Food> food;

		static bool isInfected;
		
		static uint frameCount;
		
		static uint secCount;
		
		/* --- PRIVATE FUNCTIONS --- */

		// Initialises the data members
		void init_variables(c_TileType tileType = VOID_TILE, c_uint rows = 0, c_uint cols = 0);

		// Initialises the food
		void init_food();

		// Returns the bounds of the tile
		sf::FloatRect get_bounds();

		bool is_out_of_bounds();

		// Returns true if the two tiles are the same
		bool compare(Tile* nextTile);

		// Returns true if the tile is restricted
		bool is_restricted_area(bool canEnterMonsterHouse);

		// Resets the game to its initial state
		void reset(c_uint& i);

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Tile();
		Tile(c_TileType& tileType, c_uint& rows, c_uint& cols);
		// ~Tile();

		Tile(const std::shared_ptr<Tile>& other);

		// Tile(const Tile& other);
		// Tile& operator=(Tile other);
		// Returns the coordinates of the tile's centre
		const sf::Vector2f get_tile_centre() const;

		// Draws each data member
		void render(sf::RenderTarget* target);

		// Start the infection process of a tile
		void get_infected();

		// Update the tile timer 
		static void update_timer();

		// Get the information whether or not the tile is infected
		bool get_is_infected();
	
	// Keeps the data members and functions private
	friend class Character;
	friend class Player;
	friend class Monster;
	friend class Grid;
	friend class Game;
};

#endif // !TILE_HPP