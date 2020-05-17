#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

#include "Grid.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include "Score.hpp"
#include "Text.hpp"

// Main class
class Game {

	private:
		/* --- PRIVATE DATA MEMBERS --- */

		// Window
		std::shared_ptr<sf::RenderWindow> window;

		// Grid
		std::shared_ptr<Grid> grid;

		// Characters
		std::shared_ptr<Player> player;
		std::map<string, std::shared_ptr<Monster>> monsters;

		// Score
		std::shared_ptr<Score> score;

		// Text
		std::unique_ptr<Text> text;

		sf::Text mid_text;

		sf::Font font;
				
		/* --- PRIVATE FUNCTIONS --- */
		
		// Initialises the window its settings
		void init_window();

		// Initialises the whole grid
		void init_grid();	

		// Initialises the score
		void init_score();	

		// Initialises the player (Pac-Man)
		void init_player();

		// Initialises the foor monsters
		void init_monsters();

		// Write Ready?! before game start
		void init_text();

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
		
		// Launches the main loop
		void run();
};

#endif // !GAME_HPP