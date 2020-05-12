#include <iostream>

#include "Character.hpp"
#include "utils.hpp"

void Character::init_player(Grid* grid, c_double movementSpeed, c_double centreX, c_double centreY) {
	this->movementSpeed = movementSpeed;
	this->centreX = centreX;
	this->centreY = centreY;
	this->dirX = this->dirY = this->prevDirX = this->prevDirY = 0;
	this->takeCorner = takeRightAngle = false;
	this->nextTurnDirX = this->nextTurnDirY = 0;
	this->grid = grid;
	this->aboveTile = nullptr;
	this->digestCooldown = 0;
}

void Character::init_monster(Grid* grid, c_string& name) {
	this->movementSpeed = 0.95f * REF_SPEED;
	this->dirX = this->dirY = this->prevDirX = this->prevDirY = 0;
	this->grid = grid;
	this->aboveTile = nullptr;

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

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Character::Character(Grid* grid) {
	init_player(grid, REF_SPEED, 14.f * CELL_SIZE, 26.5f * CELL_SIZE);
}

Character::Character(Grid* grid, c_string& name) {
	init_monster(grid, name);
}

Character::~Character() {}

// bool Character::handle_wall_collisions(Tile* nextTile) {

// 	if(nextTile->is_wall()) {
// 		////std::cout << "wall here\n";
// 		if(is_changing_axis()) {
// 			// std::cout << "might corner soon.\n";
// 			takeCorner = true;

// 			nextTurnDirX = dirX;
// 			nextTurnDirY = dirY;

// 			this->dirX = prevDirX;
// 			this->dirY = prevDirY;

// 			// std::cout << "Prev x y: " << prevDirX << " " << prevDirY << "\n";
// 			// std::cout << "Curr x y: " << dirX << " " << dirY << "\n";
// 			// std::cout << "NextTurn x y: " << nextTurnDirX << " " << nextTurnDirY << "\n\n";
// 			return false;
// 		}
// 		// Right
// 		if(dirX == 1 && (centreX + movementSpeed) >= aboveTile->get_tile_centre().x) {
// 			centreX = aboveTile->get_tile_centre().x;

// 			return true;
// 		}

// 		// Left
// 		if(dirX == -1 && (centreX - movementSpeed) <= aboveTile->get_tile_centre().x) {
// 			centreX = aboveTile->get_tile_centre().x;
// 			return true;
// 		}

// 		// Down
// 		if(dirY == 1 && (centreY + movementSpeed) >= aboveTile->get_tile_centre().y) {
// 			centreY = aboveTile->get_tile_centre().y;
// 			return true;
// 		}

// 		// Up
// 		if(dirY == -1 && (centreY - movementSpeed) <= aboveTile->get_tile_centre().y) {
// 			centreY = aboveTile->get_tile_centre().y;
// 			return true;
// 		}
// 		// Update things
// 	}

// 	return false;
// }

// bool Character::is_cornering_completed(Tile* cornerDestTile, c_double& nextCentreX, c_double& nextCentreY) {

// 	if(dirX == 1 && nextCentreX >= cornerDestTile->get_bounds().left + CELL_SIZE / 2.f)
// 		return true;

// 	else if(dirX == -1 && nextCentreX <= cornerDestTile->get_bounds().left + CELL_SIZE / 2.f)
// 		return true;
	
// 	else if(dirY == 1 && nextCentreY >= cornerDestTile->get_bounds().top + CELL_SIZE / 2.f)
// 		return true;

// 	else if(dirY == -1 && nextCentreY <= cornerDestTile->get_bounds().top + CELL_SIZE / 2.f)
// 		return true;

// 	return false;
// }

// void Character::handle_cornering() {
// 	int corneringDirX, corneringDirY;

// 	if(dirX == 0) {
// 		corneringDirX = nextTurnDirX;
// 		corneringDirY = dirY;
// 	}
// 	else {
// 		corneringDirX = dirX;
// 		corneringDirY = nextTurnDirY;
// 	}

// 	centreX += movementSpeed * corneringDirX;
// 	centreY += movementSpeed * corneringDirY;
// 	shape->move(movementSpeed * corneringDirX, movementSpeed * corneringDirY);

// 	std::cout << "\n\nCentre: " << centreX << " " << centreY << "\n\n";

// 	Tile* cornerDestTile = find_next_tile(nextTurnDirX, nextTurnDirY);

// 	if(is_cornering_completed(cornerDestTile, centreX + movementSpeed * corneringDirX,
// 				centreY + movementSpeed * corneringDirY)){

// 		//shape->move(movementSpeed * corneringDirX, movementSpeed * corneringDirY);
// 		std::cout << "\n\nDO YOU GO HERE ?\n\n";

// 		centreX = cornerDestTile->get_tile_centre().x;
// 		centreY = cornerDestTile->get_tile_centre().y;

// 		shape->setPosition(centreX, centreY);

// 		takeCorner = false;

// 		prevDirX = dirX = nextTurnDirX;
// 		prevDirY = dirY = nextTurnDirY;

// 		// Recentrer pacman pour éviter l'accumulations d'erreurs d'arrondis
// 		// if(nextTurnDirX != 0)
// 		// 	centreY = cornerDestTile->get_bounds().top + CELL_SIZE / 2.f;
// 		// else if(nextTurnDirY != 0)
// 		// 	centreX = cornerDestTile->get_bounds().left + CELL_SIZE / 2.f;

// 		aboveTile = cornerDestTile;
// 	}


	

	

// 	// std::cout << "\nabove:\n\t";
// 	// std::cout << "Tiletype: " << aboveTile->tileType << "\n\t";
// 	// std::cout << "Row: " << aboveTile->rows << "\n\t";
// 	// std::cout << "Cols: " << aboveTile->cols << "\n";
// 	// std::cout << "Dir: " << dirX << " " << dirY << "\n";
// 	// std::cout << "Prev dir: " << prevDirX << " " << prevDirY << "\n\n";

// 	// std::cout << "cornerDestTile:\n\t";
// 	// std::cout << "Tiletype: " << cornerDestTile->tileType << "\n\t";
// 	// std::cout << "Row: " << cornerDestTile->rows << "\n\t";
// 	// std::cout << "Cols: " << cornerDestTile->cols << "\n";
// 	// std::cout << "Dir: " << dirX << " " << dirY << "\n";
// 	// std::cout << "Prev dir: " << prevDirX << " " << prevDirY << "\n";
// 	// std::cout << "cornering dir: " << corneringDirX << " " << corneringDirY << "\n";
// 	// std::cout << "next dir: " << nextTurnDirX << " " << nextTurnDirY << "\n";
// }

// bool Character::handle_turn() {
// 	// Si takeCorner = true (&& character bien positionné)-> handle cornering

// 	Tile* adjTile = find_next_tile(nextTurnDirX, nextTurnDirY);

// 	// if(nextTurnDirX == 0 && nextTurnDirY == -1) {

// 	// 	std::cout << "above:\n\t";
// 	// 	std::cout << "Tiletype: " << aboveTile->tileType << "\n\t";
// 	// 	std::cout << "Row: " << aboveTile->rows << "\n\t";
// 	// 	std::cout << "Cols: " << aboveTile->cols << "\n";
// 	// 	std::cout << "Dir: " << dirX << " " << dirY << "\n";
// 	// 	std::cout << "Prev dir: " << prevDirX << " " << prevDirY << "\n";

// 	// 	std::cout << "\n\n\nAdjTile:\n\t";
// 	// 	std::cout << "Tiletype: " << adjTile->tileType << "\n\t";
// 	// 	std::cout << "Row: " << adjTile->rows << "\n\t";
// 	// 	std::cout << "Cols: " << adjTile->cols << "\n";
// 	// 	std::cout << "Dir: " << dirX << " " << dirY << "\n";
// 	// 	std::cout << "Prev dir: " << prevDirX << " " << prevDirY << "\n\n\n";
// 	// }

// 	if(takeCorner && !adjTile->is_wall()) {

	
// 		handle_cornering();

// 		return true;
// 	}

// 	// Case right angle (when pacman already reached the tile when the command is triggered)

// 	return false;
// }

// bool Character::handle_treat_collision(Tile* nextTile, c_double nextCentreX, c_double nextCentreY) {
	

// 	if(nextTile->tileType == TREAT_TILE && 
// 		nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY))) {

// 		// Remove the food from remainingFood at a specific index
// 		grid->remove_food(nextTile->rows, nextTile->cols);
					
// 		// digestion time = duration of a whole frame (in msec)
// 		// 2 * 16: Not very consistant but the update() is called juste after move() 
// 		// We need to finish the current frame, block moves for the following frame and then
// 		// resume moving
		
// 		aboveTile = nextTile;

// 		centreX = nextCentreX;
// 		centreY = nextCentreY;
		
// 		prevDirX = dirX;
// 		prevDirY = dirY;

// 		shape->move(movementSpeed * dirX, movementSpeed * dirY);

// 		digestCooldown = 32;

// 		// Update counter (eaten food)

// 		// Update score

// 		// Digest !!!
// 		return true;
// 	}

// 	return false;
// }

void Character::set_direction(c_int dirX, c_int dirY) {
	this->dirX = dirX;
	this->dirY = dirY;
}

















Tile* Character::find_next_tile(c_int& dirX, c_int& dirY) {
	return grid->get_tile_at(aboveTile->rows + dirY, aboveTile->cols + dirX);
}

bool Character::is_digesting() {
	return digestCooldown > 0;
}

bool Character::handle_initial_move() {
	// 'aboveTile' is nullptr at the beginning of the game
	if(aboveTile)
		return false;

	// Only horizontal movements can change this state (nullptr)

	if(dirX == 1)
		aboveTile = grid->get_tile_at(26, 14);

	else if(dirX == -1)
		aboveTile = grid->get_tile_at(26, 13);

	else
		return true;

	centreX += movementSpeed * dirX;
	centreY += movementSpeed * dirY;

	shape->move(movementSpeed * dirX, movementSpeed * dirY);

	prevDirX = dirX;
	prevDirY = dirY;

	return true;
}

bool Character::handle_tunnel() {

	if(is_tunnel(aboveTile->rows + dirY, aboveTile->cols + dirY)) {
		// Si nextMove est hors d'aboveTile -> téléporter
		// Sinon, on avance

		return true;
	}

	return false;
}

bool Character::handle_blocking_wall() {

	if(is_changing_axis()) {
		// std::cout << "hi\n";
		return false;
	}

	// Right
	if(dirX == 1 && (centreX + movementSpeed) >= aboveTile->get_tile_centre().x) 
		centreX = aboveTile->get_tile_centre().x;
	// Left
	else if(dirX == -1 && (centreX - movementSpeed) <= aboveTile->get_tile_centre().x)
		centreX = aboveTile->get_tile_centre().x;
	// Down
	else if(dirY == 1 && (centreY + movementSpeed) >= aboveTile->get_tile_centre().y) 
		centreY = aboveTile->get_tile_centre().y;
	//Up
	else if(dirY == -1 && (centreY - movementSpeed) <= aboveTile->get_tile_centre().y)
		centreY = aboveTile->get_tile_centre().y;
	else {
		centreX += movementSpeed * dirX;
		centreY += movementSpeed * dirY;

		prevDirX = dirX;
		prevDirY = dirY;

		shape->move(centreX, centreY);
	}

	return true;	
}

bool Character::handle_non_blocking_wall() {
	if(!is_changing_axis())
		return false;

	Tile* nextTile = find_next_tile(prevDirX, prevDirY);

	takeCorner = true;

	nextTurnDirX = dirX;
	nextTurnDirY = dirY;

	dirX = prevDirX;
	dirY = prevDirY;

	float nextCentreX = centreX + movementSpeed * dirX;
	float nextCentreY = centreY + movementSpeed * dirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);
	
	if(nextTile->get_bounds().contains(nextCentre)) {
		aboveTile = nextTile;

		if(dirX == -1) {
			std::cout << "a\n";
			centreX = nextTile->get_bounds().left + CELL_SIZE;
			centreY = nextTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(dirX == 1) {
			std::cout << "b\n";
			centreX = nextTile->get_bounds().left;
			centreY = nextTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(dirY == 1) {
			std::cout << "c\n";
			centreX = nextTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = nextTile->get_bounds().top;
		}
		else if(dirY == -1) {
			std::cout << "d\n";
			centreX = nextTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = nextTile->get_bounds().top + CELL_SIZE;
		}

		shape->setPosition(centreX, centreY);

		dirX = nextTurnDirX;
		dirY = nextTurnDirY;

		return true;
	}

	centreX = nextCentreX;
	centreY = nextCentreY;

	shape->move(movementSpeed * dirX, movementSpeed * dirY);

	dirX = nextTurnDirX;
	dirY = nextTurnDirY;

	return true;
}

bool Character::handle_wall() {
	Tile* nextTile = find_next_tile(dirX, dirY);
	if(!nextTile->is_wall())
		return false;

	// Cas où pac-man fonce dans un mur (pas de changemnet de direction)
	if(handle_blocking_wall()) {
		shape->setPosition(centreX, centreY);
		return true;
	}

	// Cas où le joueur change de direction mais la case suivante dans cette direction est un mur
	if(handle_non_blocking_wall())
		return true;

	return false;
}

bool Character::is_changing_axis() {
	return dirX != prevDirX && dirY != prevDirY;
}

bool Character::is_changing_direction() {
	return dirX != prevDirX || dirY != prevDirY;
}

bool Character::is_changing_orientation() {
	return (dirX == prevDirX && dirY != prevDirY) || (dirX != prevDirX && dirY == prevDirY);
}

bool Character::handle_treat() {
	Tile* nextTile = find_next_tile(dirX, dirY);

	if(nextTile->tileType != TREAT_TILE)
		return false;

	float nextCentreX = centreX + movementSpeed * dirX;
	float nextCentreY = centreY + movementSpeed * dirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

	if(nextTile->get_bounds().contains(nextCentre)) {
	
		grid->remove_food(nextTile->rows, nextTile->cols);
		aboveTile = nextTile;
		digestCooldown = 32;
	}

	centreX = nextCentreX;
	centreY = nextCentreY;

	prevDirX = dirX;
	prevDirY = dirY;

	shape->move(movementSpeed * dirX, movementSpeed * dirY);

	return true;
}

bool Character::is_right_angle_timing() {

	if(prevDirX == 1 && centreX <= aboveTile->get_bounds().left + CELL_SIZE / 2.f)
		return true;

	else if(prevDirX == -1 && centreX >= aboveTile->get_bounds().left + CELL_SIZE / 2.f)
		return true;

	else if(prevDirY == 1 && centreY <= aboveTile->get_bounds().top + CELL_SIZE / 2.f)
		return true;

	else if(prevDirY == -1 && centreY >= aboveTile->get_bounds().top + CELL_SIZE / 2.f)
		return true;

	else
		return false;
}

bool Character::handle_right_angle() {
	

	if(is_right_angle_timing()) {

		if(cmp_point(aboveTile->get_tile_centre(), sf::Vector2f(centreX, centreY), movementSpeed)) {

			centreX = aboveTile->get_tile_centre().x;
			centreY = aboveTile->get_tile_centre().y;
			shape->setPosition(centreX, centreY);

			prevDirX = dirX;
			prevDirY = dirY;

			shape->move(movementSpeed * dirX, movementSpeed * dirY);

			return true;
		}
		else {
			centreX += movementSpeed * prevDirX;
			centreY += movementSpeed * prevDirY;

			shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);

			return true;
		}
	}
	
	Tile* nextTile = find_next_tile(prevDirX, prevDirY);
	if(nextTile->is_wall()) {

		centreX = aboveTile->get_tile_centre().x;
		centreY = aboveTile->get_tile_centre().y;

		dirX = prevDirX;
		dirY = prevDirY;

		shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);
		return true;
	}
	else {
		float nextCentreX = centreX + movementSpeed * dirX;
		float nextCentreY = centreY + movementSpeed * dirY;
		sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

		if(nextTile->get_bounds().contains(nextCentre))
			aboveTile = nextTile;

		centreX = nextCentreX;
		centreY = nextCentreY;

		dirX = prevDirX;
		dirY = prevDirY;

		shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);

		return true;
	}

	return false;

	// Si le centre de pacman est dans la première moitié de la tile, il pourra tourner à terme

	// Sinon, deux sous-cas:
		// S'il y a un mur dans l'ancienne direction (prevDir): fixer au centre de la tile
		// Sinon, on peut avancer

}

bool Character::handle_cornering() {
	if(!takeCorner)
		return false;

	int cornerDirX, cornerDirY;

	if(prevDirX == 0) {
		cornerDirX = nextTurnDirX;
		cornerDirY = prevDirY;
	}
	else {
		cornerDirX = prevDirX;
		cornerDirY = nextTurnDirY;
	}

	Tile* cornerTile = find_next_tile(nextTurnDirX, nextTurnDirY);

	float nextCentreX = centreX + movementSpeed * cornerDirX;
	float nextCentreY = centreY + movementSpeed * cornerDirY;

	if(cornerTile->get_bounds().contains(nextCentreX, nextCentreY)) {
		aboveTile = cornerTile;
		takeCorner = false;

		if(nextTurnDirX == -1 && nextTurnDirY == 0) {
			std::cout << "1\n";
			centreX = cornerTile->get_bounds().left;
			centreY = cornerTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(nextTurnDirX == 1 && nextTurnDirY == 0) {
			std::cout << "2\n";
			centreX = cornerTile->get_bounds().left + CELL_SIZE;
			centreY = cornerTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(nextTurnDirX == 0 && nextTurnDirY == -1) {
			std::cout << "coucou\n";
			centreX = cornerTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = cornerTile->get_bounds().top;
		}
		else if(nextTurnDirX == 0 && nextTurnDirY == 1) {
			std::cout << "3\n";
			centreX = cornerTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = cornerTile->get_bounds().top + CELL_SIZE;
		}

		prevDirX = dirX = nextTurnDirX;
		prevDirY = dirY = nextTurnDirY;

		shape->setPosition(centreX, centreY);

		return true;
	}
	else {
		centreX = nextCentreX;
		centreY = nextCentreY;

		shape->move(movementSpeed * cornerDirX, movementSpeed * cornerDirY);

		return true;
	}
}

bool Character::handle_turn() {
	if(!is_changing_axis()) {
		return false;
	}
		
	// Cornering case
	if(handle_cornering()) {
		return true;
	}
		
	// RIght angle case
	if(handle_right_angle())
		return true;

	


	return false;

}

void Character::move() {
	// Digesting takes a whole frame when Pac-Man cannot move
	if(is_digesting()) {

		return;
	}
		

	// Special case: Pac-Man begins between two tiles
	if(handle_initial_move())
		return;

	// std::cout << "AboveTile:\n\t";
	// std::cout << "TileType: " << aboveTile->tileType << "\n\t";
	// std::cout << "Rows: " << aboveTile->rows << "\n\t";
	// std::cout << "Cols: " << aboveTile->cols << "\n";

	// If the next tile is in the tunnel
	if(handle_tunnel())
		return;

	if(handle_wall())
		return;

	// if(takeCorner == true)
	// 	return;

	if(handle_turn())
		return;

	if(handle_treat())
		return;

	

	else {

		Tile* nextTile = find_next_tile(dirX, dirY);
		float nextCentreX = centreX + movementSpeed * dirX;
		float nextCentreY = centreY + movementSpeed * dirY;

		// std::cout << "NextCentre: " << nextCentreX << " " << nextCentreY << "\n";
		// std::cout << "Centre: " << centreX << " " << centreY << "\n";

		if(nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY)))
			aboveTile = nextTile;

		centreX += movementSpeed * dirX;
		centreY += movementSpeed * dirY;
		
		shape->move(movementSpeed * dirX, movementSpeed * dirY);
		
		prevDirX = dirX;
		prevDirY = dirY;

		return;
	}
	

































	// // Cannot move while digesting
	// if(digestCooldown > 0) 
	// 	return;

	// // Special case: at the beginning of the game, the characters are positioned between two tiles
	// // Hence the aboveTile = nullptr
	// if(!aboveTile) {
	// 	handle_initial_move();
	// 	return;
	// }	

	// // std::cout << "above:\n\t";
	// // std::cout << "Tiletype: " << aboveTile->tileType << "\n\t";
	// // std::cout << "Row: " << aboveTile->rows << "\n\t";
	// // std::cout << "Cols: " << aboveTile->cols << "\n";
	// // std::cout << "Dir: " << dirX << " " << dirY << "\n";
	// // std::cout << "Prev dir: " << prevDirX << " " << prevDirY << "\n";



	// // To implement later. See what's best for the underground implementation
	// // Next tile is the underground
	// if(in_tunnel(aboveTile->rows + dirY, aboveTile->cols + dirX)) {
	// 	// Will block pacman as soon as he reaches the last tile		
	// 	return;
	// }

	// Tile* nextTile = find_next_tile(this->dirX, this->dirY);
	// // std::cout << "NextTile:\n\t";
	// // std::cout << "Tiletype: " << nextTile->tileType << "\n\t";
	// // std::cout << "Row: " << nextTile->rows << "\n\t";
	// // std::cout << "Cols: " << nextTile->cols << "\n";

	// // Updates the flag 'takeCorner' and avoid collision with walls
	// if(handle_wall_collisions(nextTile)) {
	// 	shape->setPosition(centreX, centreY);
	// 	return;
	// }

	// // Si on arrive ici, pacman est forcément en mouvement (pas bloqué par un mur)

	// // Turns pac-man at the correct timing (right angle or cornering)
	// if(handle_turn())
	// 	return;

	// // Useful ?

	// c_double nextCentreX = centreX + movementSpeed * dirX;
	// c_double nextCentreY = centreY + movementSpeed * dirY;

	// if(handle_treat_collision(nextTile, nextCentreX, nextCentreY))
	// 	return;

	// std::cout << "line before above = next\n";

	// std::cout << "centres: " << nextCentreX << " " << nextCentreY << "\n";

	// if(nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY))) {
	// 	//std::cout << "Im in\n";
	// 	aboveTile = nextTile;
	// }

	// prevDirX = dirX;
	// prevDirY = dirY;

	// centreX = nextCentreX;
	// centreY = nextCentreY;

	// shape->move(movementSpeed * dirX, movementSpeed * dirY);
	

	// std::cout << "Tile:\n\t";
	// std::cout << "Tiletype: " << aboveTile->tileType << "\n\t";
	// std::cout << "Row: " << aboveTile->rows << "\n\t";
	// std::cout << "Cols: " << aboveTile->cols << "\n";

	

	// Changement de tile

	// //std::cout << "tiletype: " << aboveTile->tileType << "\n";	



	/* else if(nextTile->tileType == TREAT_TILE) {

		// Check if there is a collision
		centreX = movementSpeed * dirX;
		centreY = movementSpeed * dirY;

		if(this->shape->getGlobalBounds().intersects(nextTile->food->shape.getGlobalBounds())) {
			//std::cout << "hiiii\n";
		}
			

		// Remove the treat in grid->remainingFood

		// Increase score


		// Eat
		// Update things
	}

	else if(nextTile->tileType == PILL_TILE) {
		//std::cout << "DONT MESS W/ ME DUDE\n";


		// Check if there is a collision

		// remove the pill in grid->remaningFood

		// Handle power pill
		// Update things
	}

	else if(nextTile->tileType == EMPTY_TILE) {
		//std::cout << "Pfff, nothing to do. Not fun mate\n";

		// Update things though
	}

	else if(nextTile->tileType == DOOR_TILE) {
		//std::cout << "U can't enter here. Bad guy only !\n";

		// Same as for walls. We need to detect it sooner

		return;
	}

	else {
		//std::cout << "i might have forgotten something...\n";
	}

	shape->move(movementSpeed * dirX, movementSpeed * dirY); */


















	/* float nextCentreX = centreX;
	float nextCentreY = centreY;

	centreX += movementSpeed * dirX;
	centreY += movementSpeed * dirY;

	Tile* nextTile = find_next_tile();
	if(nextTile == nullptr)
		//std::cout << "hi talk plz\n";

	// nextTile can be null if a character uses the underground tunnel


	if(!aboveTile) {
		//std::cout << "First move\n";
		// Beginning of the game: pac-man is between two tiles			
		aboveTile = nextTile;
	}

	// Compare t and aboveTile
	if(!aboveTile->compare(nextTile)) {
		// Checking tile cases

		

		// Underground tunnet
		if(!nextTile) {
			//std::cout << "Here's the tunnel boiii\n";
			// Update things
		}

		else if(nextTile->is_wall()) {
			//std::cout << "There's a wall there, funny guy\n";
			// Update things

			// Not fully working. Collisions must be detected sooner
			// Otherwise, characters can move in both direction even if there is only one tile large
			centreX = nextCentreX;
			centreY = nextCentreY;

			return;
		}

		else if(nextTile->tileType == TREAT_TILE) {
			//std::cout << "Miam miam\n";

			// Remove the treat in grid->remainingFood

			// Increase score


			// Eat
			// Update things
		}

		else if(nextTile->tileType == PILL_TILE) {
			//std::cout << "DONT MESS W/ ME DUDE\n";

			// remove the pill in grid->remaningFood

			// Handle power pill
			// Update things
		}

		else if(nextTile->tileType == EMPTY_TILE) {
			//std::cout << "Pfff, nothing to do. Not fun mate\n";

			// Update things though
		}

		else if(nextTile->tileType == DOOR_TILE) {
			//std::cout << "U can't enter here. Bad guy only !\n";

			// Same as for walls. We need to detect it sooner
			centreX = nextCentreX;
			centreY = nextCentreY;

			return;
		}

		else {
			//std::cout << "i might have forgotten something...\n";
		}

	}

	// Need to update aboveTile

	shape->move(movementSpeed * dirX, movementSpeed * dirY); */
}

void Character::render(sf::RenderTarget* target) {

	target->draw(*shape);
}