#include <SFML/Graphics.hpp>

#include "Monster.hpp"

/* --- PRIVATE FUNCTIONS --- */


void Monster::init_variables(c_string name) {
	this->name = name;
}


void Monster::init_shape(c_string& name) {

	c_uint pxSize = 1.6f * CELL_SIZE;

	shape = new sf::RectangleShape(sf::Vector2f(pxSize, pxSize));
	shape->setOrigin(pxSize / 2, pxSize / 2);

	if(!name.compare("Blinky")) {
		shape->setPosition(14 * CELL_SIZE, 14.5f * CELL_SIZE);
		shape->setFillColor(sf::Color::Red);
	}
		
	else if(!name.compare("Pinky")) {
		shape->setPosition(14 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color(250, 197, 246));
	}
		
	else if(!name.compare("Inky")) {
		shape->setPosition(12 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color::Cyan);
	}
	else if(!name.compare("Clyde")) {
		shape->setPosition(16 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color(247, 187, 20));
	}
}
/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Monster::Monster(Grid* grid, c_string& name) : Character(grid, name) {
	init_variables(name);
	init_shape(name);
}


Monster::~Monster() {
	delete shape;
}


void Monster::update() {
	// Changes the position, colour, ...

}