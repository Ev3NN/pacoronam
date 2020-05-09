#include "Character.hpp"
#include "constants.hpp"

// Public functions

void Character::init_variables(c_float movementSpeed) {
	this->movementSpeed = movementSpeed;
	this->dirX = this->dirY = 0.f;
	this->aboveTile = nullptr;
}

Character::Character(c_float& movementSpeed) {
	init_variables(movementSpeed);
} 

Character::~Character() {
	delete aboveTile;
}