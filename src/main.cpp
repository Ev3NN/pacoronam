#include <SFML/Graphics.hpp>
#include <ctime>

#include "Game.hpp"

int main() {
	std::srand(time(NULL));
	
	Game game;
	game.run();

	return 0;
}