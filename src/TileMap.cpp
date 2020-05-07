#include <iostream>

#include "TileMap.hpp"
#include "Tile.hpp"
#include "FoodTile.hpp"
#include "MapBuilder.hpp"

bool TileMap::load_tiles() {

	std::vector<TileType> tilesetKeys = load_tileset_keys();
	if(tilesetKeys.empty())
		return false;

	for(std::size_t i = 0; i < GRID_HEIGHT; ++i) {
		for(std::size_t j = 0; j < GRID_WIDTH; ++j) {

			TileType tileKey = tilesetKeys[i * GRID_WIDTH + j];

			if(tileKey == TREAT_TILE || tileKey == PILL_TILE) {

				FoodTile currTile(tileKey, i, j);




				grid[i][j] = currTile;

				std::pair<uint, uint> currPair(i, j);
				remainingFood.insert(std::pair<std::pair<uint, uint>, FoodTile>(currPair, currTile));
			}
				
			else
				grid[i][j] = Tile(tilesetKeys[i * GRID_WIDTH + j]);
		}
	}

	return true;
}

