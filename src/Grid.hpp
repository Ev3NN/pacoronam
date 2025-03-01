#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

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
		sf::RenderTexture canvas;
		sf::Sprite sprite;
		std::vector<TileType> tilesetKeys;
		sf::VertexArray vertices;

		// Map


		std::shared_ptr<Tile> map[GRID_ROWS][GRID_COLS];
		std::vector<std::shared_ptr<Tile>> foodTiles;

		/* --- PRIVATE FUNCTIONS --- */

		// Initialises the texture. Returns false if an error occured
		bool init_std_texture();

		void init_canvas();

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
	
	public:

		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Grid();

		//Returns the Tile or FoodTile given the specified coordinates
		std::shared_ptr<Tile> get_tile_at(c_uint& i, c_uint& j);

		// Transform the FoodTile given the specified coordinates into an EMPTY_CELL
		void remove_food(c_int& i, c_int& j);

		// Updates each data member
		void update();
		
		// Resets the game to its initial state
		void reset();

		// Draws each data member
		void render(sf::RenderTarget* target);
};


#endif // !GRID_HPP