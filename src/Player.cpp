#include "Player.hpp"
#include "constants.hpp"

// Private functions
void Player::init_shape() {

	shape = sf::CircleShape(0.8f * CELL_SIZE);
	
	shape.setFillColor(sf::Color::Yellow);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(14 * CELL_SIZE, 26.5f * CELL_SIZE);	
}

// Public functions

// Constructors

Player::Player() : Character(5 * CELL_SIZE / 60) {
	init_shape();
}

void Player::update() {
	// Changes the position, colour, ...
}

void Player::move(c_float& dirX, c_float& dirY) {
	shape.move(movementSpeed * dirX, movementSpeed * dirY);
}

void Player::render(sf::RenderTarget* target) {
	target->draw(shape);
}