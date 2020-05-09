#include <iostream>

#include "Character.hpp"
#include "constants.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
// Public functions



void Character::init_variables(c_float movementSpeed, c_float centreX, c_float centreY) {
	this->movementSpeed = movementSpeed;
	this->centreX = centreX;
	this->centreY = centreY;
	this->dirX = this->dirY = 0;
	this->aboveTile = nullptr;
}

void Character::update_above_tile(Grid* grid, c_int& dirX, c_int& dirY) {

	if(!aboveTile) {
		std::cout << "Special case: beginning of the game. First valid key has been triggered.\n";

		aboveTile = grid->get_next_tile(-1, -1, dirX, dirY);
		return;
	}

	if(aboveTile->get_bounds().contains(sf::Vector2f(centreX, centreY))) {

		// std::cout << "FloatRect: \n\t" << "Left: " << aboveTile->get_bounds().left <<
		// 			"\n\t" << "Top: " << aboveTile->get_bounds().top << "\n\t" <<
		// 			"Width: " << aboveTile->get_bounds().width << "\n\t" <<
		// 			"Circle x: " << centreX << "\n\t" << "Circle y: " << centreY << "\n";


		
		return;
	}

	std::cout << "Impawtent: " << aboveTile->rows << "\n";
	std::cout << "Impawtent2: " << aboveTile->cols << "\n";
	// Updates the aboveTile since the character has moved
	Tile* tmp = grid->get_next_tile(aboveTile->rows, aboveTile->cols, dirX, dirY);
	if(tmp == nullptr) {
		std::cout << "Out of bounds !\n";

		return;
	}

	aboveTile = tmp;

	std::cout << "Moving on !\n";
	std::cout << "FloatRect: \n\t" << "Left: " << aboveTile->get_bounds().left <<
					"\n\t" << "Top: " << aboveTile->get_bounds().top << "\n\t" <<
					"Width: " << aboveTile->get_bounds().width << "\n" <<
					"Circle x: " << centreX << "\n" << "Circle y: " << centreY << "\n" <<
					"i = " << aboveTile->rows << "\n" << "j = " << aboveTile->cols << "\n";


	// // Special case: pacman is not on a tile at the beginning
	// if(aboveTile == nullptr) {
	// 	std::cout << "shaaaape: " << shape->getPosition().x << " " << shape->getPosition().y << "\n";
	// 	std::cout << "Avant i j: -1 -1\n";
	// 	aboveTile = grid->get_next_tile(-1, -1, dirX, dirY);

	// 	std::cout << "Après i j: " << aboveTile->rows << " " << aboveTile->cols << "\n";
	// 	return;
	// }
	// 	// (-1, -1): sentinel value
		
	// else if(!aboveTile->get_bounds().contains(shape->getPosition())) {
	// 	std::cout << "hi\n";

	// 	std::cout << aboveTile->get_bounds().left << " " << aboveTile->get_bounds().top << "\n";
	// 	std::cout << "Avant i j: " << aboveTile->rows << " " << aboveTile->cols << "\n";
	// 	std::cout << "shape: " << shape->getPosition().x << " " << shape->getPosition().y << "\n";
	// 	Tile* t = grid->get_next_tile(aboveTile->rows, aboveTile->cols, dirX, dirY);
	// 	if(t != nullptr)
	// 		aboveTile = t;

	// 	std::cout << "Après i j: " << aboveTile->rows << " " << aboveTile->cols << "\n";
	// }

}

Character::Character(c_float& movementSpeed, c_float& centreX, c_float& centreY) {
	init_variables(movementSpeed, centreX, centreY);
}

Character::Character(c_float& movementSpeed, c_string& name) {
	this->movementSpeed = movementSpeed;

	if(!name.compare("Blinky")) {
		centreX = 14 * CELL_SIZE;
		centreY = 14.5f * CELL_SIZE;
	}
	else if(!name.compare("Pinky")) {
		centreX = 14 * CELL_SIZE;
		centreY = 17.5f * CELL_SIZE;
	}
	else if(!name.compare("Inky")) {
		centreX = 12 * CELL_SIZE;
		centreY = 17.5f * CELL_SIZE;
	}
	else if(!name.compare("Clyde")) {
		centreX = 16 * CELL_SIZE;
		centreY = 17.5f * CELL_SIZE;
	}
}

Character::~Character() {}

void Character::move(Grid* grid, c_int dirX, c_int dirY) {

	update_above_tile(grid, dirX, dirY);

	this->dirX = dirX;
	this->dirY = dirY;

	centreX += movementSpeed * dirX;
	centreY += movementSpeed * dirY;

	// Need to update aboveTile

	shape->move(movementSpeed * dirX, movementSpeed * dirY);
}

void Character::render(sf::RenderTarget* target) {
	target->draw(*shape);
}