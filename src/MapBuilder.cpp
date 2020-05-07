#include "MapBuilder.hpp"

#include <iostream>
#include <fstream>

bool MapBuilder::load_init_map() {

	// Loads the tileset containing every tiles of PACoronam's map
	if(!texture.loadFromFile(TILESET_FILENAME)) {
		std::cout << "An error occured while loading the map.\n";
		return false;
	}

	// Loads the tileset's keys in a vector of integers
	std::vector<TileType> tilesetKeys = load_tileset_keys();
	if(tilesetKeys.empty())
		return false;

	mapVertices.setPrimitiveType(sf::Quads);
	// Four vertices per tile
	mapVertices.resize(height * width * 4);

	for(std::size_t i = 0; i < height; ++i)
		for(std::size_t j = 0; j < width; ++j) {
			
			// Tileset's key indicating the texture to use
			TileType tileKey = tilesetKeys[i * width + j];

			if(tileKey == TREAT_TILE || tileKey == PILL_TILE)
				tileKey = EMPTY_TILE;

			sf::Vertex* quad = &mapVertices[(i * width + j) * 4];

			quad[0].position = sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE);
			quad[1].position = sf::Vector2f((j + 1) * CELL_SIZE, i * CELL_SIZE);
			quad[2].position = sf::Vector2f((j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
			quad[3].position = sf::Vector2f(j * CELL_SIZE, (i + 1) * CELL_SIZE);

			// Tileset'size: 3x5 (hence the 5)
			int textureWidth = tileKey % IMAGES_NBR_PER_ROW;
			int textureHeight = tileKey / IMAGES_NBR_PER_ROW;

			quad[0].texCoords = sf::Vector2f(textureWidth * TILE_IMAGE_SIZE, textureHeight * TILE_IMAGE_SIZE);
			quad[1].texCoords = sf::Vector2f((textureWidth + 1) * TILE_IMAGE_SIZE, textureHeight * TILE_IMAGE_SIZE);
			quad[2].texCoords = sf::Vector2f((textureWidth + 1) * TILE_IMAGE_SIZE, (textureHeight + 1) * TILE_IMAGE_SIZE);
			quad[3].texCoords = sf::Vector2f(textureWidth * TILE_IMAGE_SIZE, (textureHeight + 1) * TILE_IMAGE_SIZE);
		}

	return true;
}

std::vector<TileType> load_tileset_keys() {
	std::vector<TileType> tilesetKeys;

	std::ifstream ifs(MAP_TEXTURE_KEYS_FILENAME);
	if(!ifs) {
		std::cout << "An error occured while opening " << MAP_TEXTURE_KEYS_FILENAME << ".\n";
		return tilesetKeys;
	}

	uint a;
	// The vector only stores the integers encountered
	while(ifs >> a)
		tilesetKeys.push_back((TileType) a);
		
	ifs.close();

	return tilesetKeys;
}