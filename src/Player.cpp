#include "Player.hpp"
#include "constants.hpp"

// Private functions
void Player::init_shape() {

	float radius = 0.8f * CELL_SIZE;

	shape = new sf::CircleShape(radius);
	
	shape->setFillColor(sf::Color::Yellow);
	shape->setOrigin(radius, radius);
	shape->setPosition(14 * CELL_SIZE, 26.5f * CELL_SIZE);	
}

// Public functions

// Constructors

Player::Player() : Character(5 * CELL_SIZE / 60, 14 * CELL_SIZE, 26.5f * CELL_SIZE) {
	init_shape();
}

Player::~Player() {
	delete shape;
}

void Player::update() {
	// Changes the position, colour, ...
}