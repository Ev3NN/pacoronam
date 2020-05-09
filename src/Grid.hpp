#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

#include "constants.hpp"
#include "FoodTile.hpp"
#include "Tile.hpp"

class Grid {

	private:
		// Size
		static c_uint rows = GRID_ROWS;
		static c_uint cols = GRID_COLS;

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

		Tile* get_next_tile(c_int& i, c_int& j, c_int& dirX, c_int& dirY);
	
	public:
		// Constructors/Destructor
		Grid();

		void update();

		void render(sf::RenderTarget* target);

	friend class Character;
};

#endif // !GRID_HPP