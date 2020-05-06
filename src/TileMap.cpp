#include "TileMap.hpp"
#include "constants.hpp"

#include <iostream>
#include <fstream>

bool TileMap::load_map() {

	if(!mapTexture.loadFromFile(TILESET_FILENAME)) {
		std::cout << "An error occured while loading map.\n";
		return false;
	}

	std::ifstream myFile(MAP_TEXTURE_CODES_FILENAME);
	if(!myFile) {
		std::cout << "Error while opening file\n";
		return false;
	}
		
	std::vector<int> mapTextureCodes = {};

	int a;

	while(myFile >> a) {
			std::cout << a << "\n";
			mapTextureCodes.push_back(a);
	}
	myFile.close();

	mapVertices.setPrimitiveType(sf::Quads);
	mapVertices.resize(GRID_HEIGHT * GRID_WIDTH * 4);

	for(size_t i = 0; i < GRID_HEIGHT; ++i) {
		for(size_t j = 0; j < GRID_WIDTH; ++j) {
			
			char tileTextureCode = mapTextureCodes[i * GRID_WIDTH + j];
			if(i == 3) {
				std:: cout << "i, j, code: " << i << " " << j << " " << mapTextureCodes[i * GRID_WIDTH + j] << "\n";
			}

			sf::Vertex* quad = &mapVertices[(i * GRID_WIDTH + j) * 4];

			quad[0].position = sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE);
			quad[1].position = sf::Vector2f((j + 1) * CELL_SIZE, i * CELL_SIZE);
			quad[2].position = sf::Vector2f((j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
			quad[3].position = sf::Vector2f(j * CELL_SIZE, (i + 1) * CELL_SIZE);

			int textureWidth = tileTextureCode % 5;
			int textureHeight = tileTextureCode / 5;

			quad[0].texCoords = sf::Vector2f(textureWidth * TILE_IMAGE_SIZE, textureHeight * TILE_IMAGE_SIZE);
			quad[1].texCoords = sf::Vector2f((textureWidth + 1) * TILE_IMAGE_SIZE, textureHeight * TILE_IMAGE_SIZE);
			quad[2].texCoords = sf::Vector2f((textureWidth + 1) * TILE_IMAGE_SIZE, (textureHeight + 1) * TILE_IMAGE_SIZE);
			quad[3].texCoords = sf::Vector2f(textureWidth * TILE_IMAGE_SIZE, (textureHeight + 1) * TILE_IMAGE_SIZE);

		}
	}

	return true;
}