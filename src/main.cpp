#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "constants.hpp"
#include "MapBuilder.hpp"
#include "TileMap.hpp"
#include "Tile.hpp"
#include "FoodTile.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE), "PACoronam");	

	TileMap tileMap;
	if(!tileMap.load_tiles())
		return 1;

	MapBuilder mapBuilder;
	if (!mapBuilder.load_init_map())
		return 1;
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if(event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(mapBuilder);
		window.draw(tileMap);
		window.display();
	}

	return 0;
}