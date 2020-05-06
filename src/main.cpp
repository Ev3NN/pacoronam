#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "TileMap.hpp"
#include "constants.hpp"

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_SIZE + 5, GRID_HEIGHT * CELL_SIZE + 5), "Tilemap");

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load_map())
        return -1;

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        window.clear();
		
        window.draw(map);
        window.display();
    }

    return 0;
}