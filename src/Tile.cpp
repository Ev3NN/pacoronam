#include "Tile.hpp"

Tile::Tile() {}
Tile::Tile(TileType tileType) : tileType(tileType), pxSize(CELL_SIZE) {}