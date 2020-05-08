#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <map>

#include "Grid.hpp"
#include "Player.hpp"
#include "Monster.hpp"

class Game {

	private:
		// Window
		sf::RenderWindow* window;

		// Grid
		Grid *grid;
		Player* player;
		// Map of monsters
		std::map<std::string, Monster> monsters;
		
		// Private functions
		void init_window();
		void init_grid();	
		void init_player();
		void init_monsters();

	public:
		Game();
		~Game();
		
		// Public functions
		void run();

		void update_poll_events();
		void update_input();
		void update();

		void render();
};

#endif // !GAME_HPP