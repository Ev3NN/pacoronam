#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>

// Main class
class Text {

	private:
		/* --- PRIVATE DATA MEMBERS --- */

		sf::Font font;
		
		sf::Text text;

        std::string word;

		/* --- PRIVATE FUNCTIONS --- */

        // Initialise the display the game text
		void init_text();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructor
		Text();

		// Render the text
		void render(sf::RenderTarget* target);

        // Set the text to empty text
        void in_game_text();

        // Set the text to victory text
        void win_text();

        // Set the text to defeat text
        void loose_text();

		// Reset the score
		void reset();
        
};

#endif // !TEXT_HPP