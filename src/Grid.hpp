#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.hpp"
#include "Tile.hpp"

// Represents the map
class Grid {

	private:
		/* --- PRIVATE DATA MEMBERS --- */

		// Sizes
		static c_uint rows = GRID_ROWS;
		static c_uint cols = GRID_COLS;

		// Visual apperance
		sf::Texture texture;
		std::vector<TileType> tilesetKeys;
		sf::VertexArray vertices;

		// Map
		
		

		/* --- PRIVATE DATA MEMBERS --- */

		// Initialises the texture. Returns false if an error occured
		bool init_std_texture();

		// Initialises the texture in DEBUG mode. Returns false if an error occured
		bool init_debug_texture();

		// Initialises the texture in STANDARD mode. Returns false if an error occured
		bool init_texture();

		// Initialises the file containing the tileset's keys. Returns false if an error occured
		bool init_tileset_keys();

		// Initialises the array of vertices
		void init_vertices();

		// Initialises the map
		void init_map();

		// Returns a specific tile after a movement
		// Tile* get_next_tile(c_int& i, c_int& j, c_int& dirX, c_int& dirY);
	
	public:

		Tile* map[GRID_ROWS][GRID_COLS];
		std::vector<Tile*> remainingFood;
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Grid();
		~Grid();

		//Returns the Tile or FoodTile at a specified coordinate
		Tile* get_tile_at(c_uint& i, c_uint& j);

		void remove_food(c_uint& i, c_uint& j);

		// Not implemented yet !
		// Updates each data member
		void update();

		void reset();

		// Draws each data member
		void render(sf::RenderTarget* target);

	// Need to refactor this
	// Character needs to computes the next tile to update its data members
};

#endif // !GRID_HPP