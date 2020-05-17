#include <SFML/Graphics.hpp>
#include <iostream>

#include "constants.hpp"
#include "Text.hpp"

using namespace std;

/* --- PUBLIC FUNCTIONS --- */

void Text::init_text() {
    if(!font.loadFromFile(FONT_FILENAME)) {
		std::cerr << "couldn't load from " << FONT_FILENAME << std::endl;
		system("pause");
	}
	text.setFont(font);

    word = "Ready?!";
    int halfWord = (text.getCharacterSize() * word.length())/2;

    text.setCharacterSize(32);
	text.setPosition ((CELL_SIZE * (GRID_COLS/2)) - halfWord, CELL_SIZE*((GRID_ROWS/2) + 2));
	text.setFillColor(sf::Color(247, 192, 158));
	text.setString(word);
}

void Text::render(sf::RenderTarget* target) {
    text.setString(word);
	target->draw(text);
}

void Text::in_game_text() {
    word = " ";
}

void Text::win_text() {
    word = "You win!";
}

void Text::loose_text() {
    word = "Too bad!";
}

void Text::reset() {
	word = "Ready?!";
}

// Constructor
Text::Text() {
	init_text();
}
