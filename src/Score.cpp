#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "constants.hpp"
#include "Score.hpp"

using namespace std;

/* --- PUBLIC FUNCTIONS --- */

void Score::food_eat(){
	points += 10;
	str_points_update();
	text.setString(strPoints);
}

void Score::pill_eat() {
	points += 50;
	str_points_update();
	text.setString(strPoints);
}

void Score::monster_eat() {
	points += 100 * pow(2, eatenMonsterCount);
	++eatenMonsterCount;
	str_points_update();
	text.setString(strPoints);
}

void Score::end_panic() {
	eatenMonsterCount = 0;
}

void Score::reset() {
	points = 0;
	eatenMonsterCount = 0;
	strPoints = "0000";
	text.setString(strPoints);
}

void Score::str_points_update() {
	strPoints = "";
	string tmp = to_string(points);
    size_t nullChar = 4 - tmp.length();

    for(size_t i = 0; i < nullChar; ++i)
        strPoints += "0";

    strPoints += tmp;
}

void Score::init_score_display() {
    if(!font.loadFromFile(FONT_FILENAME)) {
		std::cerr << "couldn't load from " << FONT_FILENAME << std::endl;
		system("pause");
	}
	text.setFont(font);

	int halfWord = (text.getCharacterSize() * strPoints.length())/2;

	text.setCharacterSize(32);
	text.setPosition ((CELL_SIZE * (GRID_COLS/2) - halfWord), CELL_SIZE);
	text.setFillColor(sf::Color(247, 192, 158));
	text.setString(strPoints);
}

void Score::render(sf::RenderTarget* target) {
	target->draw(text);
}

// Constructor
Score::Score() {
	points = 0;
	eatenMonsterCount = 0;
	strPoints = "0000";
	init_score_display();
}
