#include <iostream>
#include <fstream>

#include "Grid.hpp"
#include "utils.hpp"

/* --- PRIVATE FUNCTIONS --- */

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
		tilesetKeys.push_back((TileType) a);
		
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
	for(std::size_t i = 0; i < rows; ++i) {
		for(std::size_t j = 0; j < cols; ++j) {

			TileType tileKey = tilesetKeys[i * cols + j];

			auto newTile = std::make_shared<Tile>(tileKey, i, j);

			map[i][j] = newTile;

			if(tileKey == TREAT_TILE || tileKey == PILL_TILE)
				// Adds a food tile in the map and in foodTiles
				foodTiles.push_back(newTile);
		}
	}
}

/* --- PUBLIC FUNCTIONS --- */

// Constructor & Destructor
Grid::Grid() {

	// Does not handle potential errors !!! -> Exceptions ?
	if(!init_texture())
		std::cout << "Do something 'bout that.\n";
	if(!init_tileset_keys())
		std::cout << "Do something 'bout that.\n";

	init_vertices();
	init_map();
}

std::shared_ptr<Tile> Grid::get_tile_at(c_uint& i, c_uint& j) {
	return map[i][j];
}

void Grid::remove_food(c_int& iFood, c_int& jFood) {
	if(is_out_of_bounds(iFood, jFood))
		return;

	for(auto foodTile : foodTiles) {
		// Find the food tile we want
		// Memory leaks or double free if I try to do this properly (using erase and delete)
		if(foodTile->rows == iFood && foodTile->cols == jFood) {
			foodTile->tileType = EMPTY_TILE;
			//std::cout << "food reset: " << foodTile->tileType << "\n";
			return;
		} 
	}

}

void Grid::update() {

	// Modifier map en fonction de foodTiles entre autres
}

void Grid::reset() {
	for(std::size_t i = 0; i < foodTiles.size(); ++i)
		foodTiles[i]->reset(i);
}


void Grid::render(sf::RenderTarget* target) {

	target->draw(vertices, &texture);

	for(auto &foodTile : foodTiles)
		if(foodTile->tileType == TREAT_TILE || foodTile->tileType == PILL_TILE)
			foodTile->render(target);
}