#include "Tile.hpp"

#include <iostream>
#include <fstream>

Tile::Tile() {}

Tile::Tile(TileType tileType) : tileType(tileType) {}

Tile& Tile::operator = (const Tile& other) {

    if(&other == this)
        return *this;

    tileType = other.tileType;

    return *this;
}