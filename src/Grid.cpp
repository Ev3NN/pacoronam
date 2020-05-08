#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <utility>

#include "Grid.hpp"
#include "Tile.hpp"

// Private functions

bool Grid::init_std_texture() {
	texture = sf::Texture();
	if(!texture.loadFromFile(STD_TILESET_FILENAME))
		return false;

	texture.setSmooth(true);
	return true;
}

bool Grid::init_debug_texture() {
	texture = sf::Texture();
	if(!texture.loadFromFile(DEBUG_TILESET_FILENAME))
		return false;

	texture.setSmooth(true);
	return true;
}

bool Grid::init_texture() {
	if(DEBUG)
		return init_debug_texture();
	else
		return init_std_texture();
}

bool Grid::init_tileset_keys() {

	std::ifstream ifs(TILESET_KEYS_FILENAME);
	if(!ifs) {
		std::cout << "An error occured while opening " << TILESET_KEYS_FILENAME << ".\n";
		return false;
	}

	uint a;
	// The vector only stores the integers encountered
	while(ifs >> a)
		this->tilesetKeys.push_back((TileType) a);
		
	ifs.close();

	return true;
}


void Grid::init_vertices() {
	vertices.setPrimitiveType(sf::Quads);
	// Four vertices per tile
	vertices.resize(rows * cols * 4);

	for(std::size_t i = 0; i < rows; ++i)
		for(std::size_t j = 0; j < cols; ++j) {
			TileType tileKey = tilesetKeys[i * cols + j];
			
			// The background of a food tile is an empty (black) cell
			if(tileKey == TREAT_TILE || tileKey == PILL_TILE)
				tileKey = EMPTY_TILE;

			sf::Vertex* quad = &vertices[(i * cols + j) * 4];

			quad[0].position = sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE);
			quad[1].position = sf::Vector2f((j + 1) * CELL_SIZE, i * CELL_SIZE);
			quad[2].position = sf::Vector2f((j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
			quad[3].position = sf::Vector2f(j * CELL_SIZE, (i + 1) * CELL_SIZE);

			// textureWidth = column index in tileset.png
			// textureHeight = row index in tileset.png
			uint textureColIndex = tileKey % IMAGES_NBR_PER_ROW;
			uint textureRowIndex = tileKey / IMAGES_NBR_PER_ROW;

			quad[0].texCoords = sf::Vector2f(textureColIndex * TILE_IMAGE_PX_SIZE, textureRowIndex * TILE_IMAGE_PX_SIZE);
			quad[1].texCoords = sf::Vector2f((textureColIndex + 1) * TILE_IMAGE_PX_SIZE, textureRowIndex * TILE_IMAGE_PX_SIZE);
			quad[2].texCoords = sf::Vector2f((textureColIndex + 1) * TILE_IMAGE_PX_SIZE, (textureRowIndex + 1) * TILE_IMAGE_PX_SIZE);
			quad[3].texCoords = sf::Vector2f(textureColIndex * TILE_IMAGE_PX_SIZE, (textureRowIndex + 1) * TILE_IMAGE_PX_SIZE);
		}
}

void Grid::init_map() {
	for(std::size_t i = 0; i < rows; ++i) 
		for(std::size_t j = 0; j < cols; ++j) {
			TileType tileKey = tilesetKeys[i * cols + j];

			if(tileKey == TREAT_TILE || tileKey == PILL_TILE) {
				// Adds a food tile in the map and in remainingFood
				FoodTile currTile(tileKey, i, j);

				map[i][j] = currTile;
				remainingFood.push_back(currTile);
			}
			else
				map[i][j] = Tile(tilesetKeys[i * cols + j]);
		}
}

// Public functions

// Constructors
Grid::Grid() : rows(GRID_ROWS), cols(GRID_COLS) {

	// Does not handle potential errors !!! -> Exceptions ?
	init_texture();

	init_tileset_keys();
	init_vertices();
	init_map();
}

void Grid::update() {

	// Modifier map en fonction de remainingFood entre autres


}

void Grid::render(sf::RenderTarget* target) {

	target->draw(vertices, &texture);

	for(auto &food : remainingFood)
		food.render(target);
}