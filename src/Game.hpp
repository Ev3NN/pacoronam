#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <map>

#include "Grid.hpp"
#include "Player.hpp"
#include "Monster.hpp"

// Main class
class Game {

	private:
		/* --- PRIVATE DATA MEMBERS --- */

		// Window
		sf::RenderWindow* window;

		// Grid
		Grid* grid;

		// Characters
		Player* player;
		std::map<string, Monster*> monsters;

		unsigned long long timer;
		
		/* --- PRIVATE FUNCTIONS --- */
		
		void init_variables();

		// Initialises the window its settings
		void init_window();

		// Initialises the whole grid
		void init_grid();	

		// Initialises the player (Pac-Man)
		void init_player();

		// Initialises the foor monsters
		void init_monsters();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Game();
		~Game();
		
		// Launches the main loop
		void run();

		// Notifies the program of a triggered event
		void update_poll_events();

		// Not needed anymore ?
		void update_input();

		// Updates each data member
		void update();

		// Draws each data member
		void render();
};

#endif // !GAME_HPP