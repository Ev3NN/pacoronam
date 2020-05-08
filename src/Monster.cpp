#include "Monster.hpp"
#include "constants.hpp"

// Private functions
void Monster::init_shape(std::string name) {

	c_uint pxSize = 1.6 * CELL_SIZE;

	shape = sf::RectangleShape(sf::Vector2f(pxSize, pxSize));
	shape.setOrigin(pxSize / 2, pxSize / 2);

	if(!name.compare("Blinky")) {
		shape.setPosition(14 * CELL_SIZE, 14.5 * CELL_SIZE);
		shape.setFillColor(sf::Color::Red);
	}
		
	else if(!name.compare("Pinky")) {
		shape.setPosition(14 * CELL_SIZE, 17.5 * CELL_SIZE);
		shape.setFillColor(sf::Color(250, 197, 246));
	}
		
	else if(!name.compare("Inky")) {
		shape.setPosition(12 * CELL_SIZE, 17.5 * CELL_SIZE);
		shape.setFillColor(sf::Color::Cyan);
	}
	else if(!name.compare("Clyde")) {
		shape.setPosition(16 * CELL_SIZE, 17.5 * CELL_SIZE);
		shape.setFillColor(sf::Color(247, 187, 20));
	}
}
// Public functions

// Constructors

Monster::Monster(std::string name) : Character(3.0f) {
	init_shape(name);
}

void Monster::update() {
	// Changes the position, colour, ...

}

void Monster::render(sf::RenderTarget* target) {
	target->draw(shape);
}