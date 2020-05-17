#ifndef SCORE_HPP
#define SCORE_HPP

#include <SFML/Graphics.hpp>

// Main class
class Score {

	private:
		/* --- PRIVATE DATA MEMBERS --- */

        uint eatenMonsterCount;

		uint points;

		std::string strPoints;

		sf::Font font;
		
		sf::Text text;

		/* --- PRIVATE FUNCTIONS --- */

		// Put the score in a string
		void str_points_update();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructor
		Score();

		// Score update when PAC-MAN eat a PAC-MAN food
		void food_eat();

		// Score update when PAC-MAN eat a power pill
		void pill_eat();

		// Score update when PAC-MAN eat a monster
		void monster_eat();

		// Reset the amount of eaten monsters at the end of the panic mode
		void end_panic();
        
		// Initialise the display of the score
		void init_score_display();

		// Render the score
		void render(sf::RenderTarget* target);

		// Reset the score
		void reset();
        
};

#endif // !SCORE_HPP