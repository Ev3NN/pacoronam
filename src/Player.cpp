#include "Player.hpp"
#include "constants.hpp"

// Private functions
void Player::init_shape() {

	shape = sf::CircleShape(0.8 * CELL_SIZE);
	
	shape.setFillColor(sf::Color::Yellow);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(14 * CELL_SIZE, 26.5 * CELL_SIZE);	
}

// Public functions

// Constructors

Player::Player() : Character(5.0f) {
	init_shape();	
}

void Player::update() {
	// Changes the position, colour, ...

}

void Player::render(sf::RenderTarget* target) {
	target->draw(shape);
}