#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.hpp"
#include "constants.hpp"

/* --- PRIVATE FUNCTIONS --- */


void Player::init_shape() {

	float radius = 0.8f * CELL_SIZE;

	shape = new sf::CircleShape(radius);
	
	shape->setFillColor(sf::Color::Yellow);
	shape->setOrigin(radius, radius);
	shape->setPosition(14 * CELL_SIZE, 26.5f * CELL_SIZE);	
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Player::Player(Grid* grid) : Character(grid) {
	init_shape();
}


Player::~Player() {
	delete shape;
}


void Player::update() {
	if(digestCooldown > 0)
		// 1/FPS = 16ms. The duration of an iteration in the main loop. Our cheap timer
		digestCooldown -= 16;
	else
		digestCooldown = 0;
}