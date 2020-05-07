#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <SFML/Graphics.hpp>
#include <map>

#include "Tile.hpp"
#include "FoodTile.hpp"
#include "constants.hpp"

class TileMap : public sf::Drawable {
	
	public:
		bool load_tiles();

		c_uint height = GRID_HEIGHT;
		c_uint width = GRID_WIDTH;
		Tile grid[GRID_HEIGHT][GRID_WIDTH];

		std::map<std::pair<uint, uint>, FoodTile> remainingFood;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

			for(auto itr = remainingFood.begin(); itr != remainingFood.end(); ++itr) {
				target.draw(itr->second.shape, states);
			}
		}
};

#endif