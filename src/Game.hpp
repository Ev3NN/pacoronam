#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

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
		std::shared_ptr<Grid> grid;

		// Characters
		std::shared_ptr<Player> player;
		std::map<string, std::shared_ptr<Monster>> monsters;
		
		/* --- PRIVATE FUNCTIONS --- */
		
		// Initialises the window its settings
		void init_window();

		// Initialises the whole grid
		void init_grid();	

		// Initialises the player (Pac-Man)
		void init_player();

		// Initialises the foor monsters
		void init_monsters();

		// Notifies the program of a triggered event
		void update_poll_events();

		// Notifies the Player if a specific valid key is pressed
		void update_input();

		// Updates each data member
		void update();

		// Resets the game to its initial state
		void reset();

		// Draws each data member
		void render();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Game();
		~Game();
		
		// Launches the main loop
		void run();
};

#endif // !GAME_HPP