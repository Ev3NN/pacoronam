#include "Tile.hpp"

Tile::Tile() {}
Tile::Tile(TileType tileType, uint rows, uint cols) : tileType(tileType), rows(rows),
                                                        cols(cols), pxSize(CELL_SIZE) {}

sf::Vector2f Tile::get_tile_centre() {

    float centreX = (cols + 0.5) * pxSize;
	float centreY = (rows + 0.5) * pxSize;

    return sf::Vector2f(centreX, centreY);
}