#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

#include "constants.hpp"
#include "FoodTile.hpp"

class Grid {

	private:

		// Size
		c_uint rows;
		c_uint cols;

		// Texture
		sf::Texture texture;

		// Map
		std::vector<TileType> tilesetKeys;
		Tile map[GRID_ROWS][GRID_COLS];
		sf::VertexArray vertices;

		std::vector<FoodTile> remainingFood;

		// Private functions
		bool init_std_texture();
		bool init_debug_texture();
		bool init_texture();

		bool init_tileset_keys();
		void init_vertices();
		void init_map();
	
	public:

		// Constructors/Destructor
		Grid();

		void update();

		void render(sf::RenderTarget* target);
};

#endif // !GRID_HPP