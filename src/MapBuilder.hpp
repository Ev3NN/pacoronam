#ifndef MAP_BUILDER_HPP
#define MAP_BUILDER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"

// TileMap represents the map, which contains the immutable elements (walls, doors, empty cells).
class MapBuilder : public sf::Drawable {

	public:
		// Loads the map from images and config files specified in "assets/".
		// Returns false if an error occured, true otherwise.
		bool load_init_map();

	private:
		// Draw the map in the window.
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			states.texture = &texture;
			target.draw(mapVertices, states);
		}

		// Contains the points that form the map.
		sf::VertexArray mapVertices;
		// Contains the images of tiles.
		sf::Texture texture;

		c_uint height = GRID_HEIGHT;
		c_uint width = GRID_WIDTH;
};

// Loads the tileset's keys in a vector of integers.
// Returns an empty vector if an error occured.
std::vector<TileType> load_tileset_keys();

#endif