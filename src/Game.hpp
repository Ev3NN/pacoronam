#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Grid.hpp"

class Game {

	private:
		// Window
		sf::RenderWindow* window;

		// Grid
		Grid* grid;
		
		// Private functions
		void init_window();
		void init_grid();	

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