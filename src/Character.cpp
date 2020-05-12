#include <iostream>

#include "Character.hpp"
#include "utils.hpp"

void Character::init_player(Grid* grid, c_double movementSpeed, c_double centreX, c_double centreY) {
	this->movementSpeed = movementSpeed;
	this->centreX = centreX;
	this->centreY = centreY;
	this->dirX = this->dirY = this->prevDirX = this->prevDirY = 0;
	this->takeCorner = takeRightAngle = this->isCornering = false;
	this->isMonsterHouseOpen = false;
	this->nextTurnDirX = this->nextTurnDirY = 0;
	this->grid = grid;
	this->aboveTile = nullptr;
	this->digestCooldown = 0;
}

void Character::init_monster(Grid* grid, c_string& name) {
	this->movementSpeed = 0.95f * REF_SPEED;
	this->isMonsterHouseOpen = true;
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

void Character::set_direction(c_int dirX, c_int dirY) {
	if(isCornering)
		return;

	if(is_changing_direction())
		takeCorner = false;

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
	if(is_turning())
		return false;

	// Can be put in a fonction imo

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

		// Clear nextTurDir ?

		shape->move(movementSpeed * dirX, movementSpeed * dirY);

		return true;
	}

	shape->setPosition(centreX, centreY);

	prevDirX = prevDirY = dirX = dirY = 0;

	return true;
}

bool Character::handle_non_blocking_wall() {
	if(!is_turning())
		return false;

	Tile* nextTile = find_next_tile(prevDirX, prevDirY);
	// Handles cases where the player changes the direction but at the end of the hall, 
	//there is also a wall in that new direction
	if(nextTile->is_restricted_area(isMonsterHouseOpen)) {
		// The situation becomes equivalent to moving until there is a wall in the direction of movement
		dirX = prevDirX;
		dirY = prevDirY;

		takeCorner = false;

		if(handle_blocking_wall())
			return true;
	}

	// Il n'y a pas de wall dans le sens de mouvement

	takeCorner = true;

	float nextCentreX = centreX + movementSpeed * prevDirX;
	float nextCentreY = centreY + movementSpeed * prevDirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

	if(nextTile->get_bounds().contains(nextCentre)) {
		// std::cout << "Rentre dans la cond contains\n";
		aboveTile = nextTile;

		Tile* checkTile = find_next_tile(dirX, dirY);
		if(!checkTile->is_restricted_area(isMonsterHouseOpen)) {
			// std::cout << "Rentre dans le restricted area\n";

			if(prevDirX == -1) {
				centreX = aboveTile->get_bounds().left + CELL_SIZE;
				centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
			}
			else if(prevDirX == 1) {
				centreX = aboveTile->get_bounds().left;
				centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
			}
			else if(prevDirY == 1) {
				centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
				centreY = aboveTile->get_bounds().top;
			}
			else if(prevDirY == -1) {
				centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
				centreY = aboveTile->get_bounds().top + CELL_SIZE;
			}

			shape->setPosition(centreX, centreY);

			// std::cout << "centre x: " << centreX << " " << centreY << "\n";
			
			return true;
		}

		// std::cout << "Done\n";
	}
 
	centreX = nextCentreX;
	centreY = nextCentreY;

	// std::cout << "final centre x: " << centreX << " " << centreY << "\n";

	shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);

	return true;
}

bool Character::predict_wall_collision() {
	Tile* nextTile = find_next_tile(dirX, dirY);

	return nextTile->is_restricted_area(isMonsterHouseOpen);
}

bool Character::handle_wall() {
	if(!predict_wall_collision())
		return false;

	// Cas où pac-man fonce dans un mur (pas de changemnet de direction)
	if(handle_blocking_wall())
		return true;
		
	// Cas où le joueur change de direction mais la case suivante dans cette direction est un mur
	if(handle_non_blocking_wall())
		return true;

	return false;
}

bool Character::is_turning() {
	// is_motionless needed ?
	return dirX != prevDirX && dirY != prevDirY && !is_motionless();
}

bool Character::is_changing_direction() {
	return dirX != prevDirX || dirY != prevDirY;
}

bool Character::is_changing_orientation() {
	return (dirX == prevDirX && dirY != prevDirY) || (dirX != prevDirX && dirY == prevDirY);
}

void Character::eat_treat() {
	if(!aboveTile)
		return;

	if(aboveTile->tileType != TREAT_TILE)
		return;
	
	grid->remove_food(aboveTile->rows, aboveTile->cols);
	digestCooldown = 32;
}

bool Character::is_right_angle_timing(c_float& centreX, c_float& centreY) {

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

void Character::handle_right_angle() {
	if(!is_right_angle_timing(centreX, centreY)) {

		centreX += movementSpeed * prevDirX;
		centreY += movementSpeed * prevDirY;

		Tile* nextTile = find_next_tile(prevDirX, prevDirY);
		if(nextTile->get_bounds().contains(sf::Vector2f(centreX, centreY)))
			aboveTile = nextTile;

		shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);
		
		return;
	}

	float nextCentreX = centreX + movementSpeed * prevDirX;
	float nextCentreY = centreY + movementSpeed * prevDirY;

	// Si le prochain move amène le centre de pacman plus loin qu'au milieu de la tile
	if(!is_right_angle_timing(nextCentreX, nextCentreY)) {
		
		centreX = aboveTile->get_tile_centre().x;
		centreY = aboveTile->get_tile_centre().y;
		shape->setPosition(centreX, centreY);

		prevDirX = dirX;
		prevDirY = dirY;

		return;
	}

	centreX = nextCentreX;
	centreY = nextCentreY;
	shape->move(movementSpeed * prevDirX, movementSpeed * prevDirY);
}

bool Character::handle_cornering() {
	if(!takeCorner)
		return false;

	isCornering = true;

	int cornerDirX = prevDirX + dirX;
	int cornerDirY = prevDirY + dirY;

	float nextCentreX = centreX + movementSpeed * cornerDirX;
	float nextCentreY = centreY + movementSpeed * cornerDirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

	Tile* nextTile = find_next_tile(dirX, dirY);
	if(nextTile->get_bounds().contains(nextCentre)) {
		aboveTile = nextTile;

		// Centrer
		if(dirX == -1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE;
			centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(dirX == 1) {
			centreX = aboveTile->get_bounds().left;
			centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(dirY == -1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = aboveTile->get_bounds().top + CELL_SIZE;
		}
		else if(dirY == 1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = aboveTile->get_bounds().top;
		}

		shape->setPosition(centreX, centreY);

		prevDirX = dirX;
		prevDirY = dirY;

		takeCorner = false;
		isCornering = false;

		return true;
	}

	centreX += movementSpeed * cornerDirX;
	centreY += movementSpeed * cornerDirY;


	shape->move(movementSpeed * cornerDirX, movementSpeed * cornerDirY);

	return true;
}

bool Character::handle_turn() {
	if(!is_turning()) 
		return false;
		
	// Cornering case
	if(handle_cornering())
		return true;
		
	// RIght angle case
	handle_right_angle();

	return true;
}

bool Character::is_motionless() {
	return dirX == 0 && dirY == 0;
}

void Character::move() {
	// When pac-man
	if(is_motionless())
		return;

	// Digesting takes a whole frame when Pac-Man cannot move
	if(is_digesting())
		return;
		
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

	else {


		Tile* nextTile = find_next_tile(dirX, dirY);
		float nextCentreX = centreX + movementSpeed * dirX;
		float nextCentreY = centreY + movementSpeed * dirY;

		if(nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY)))
			aboveTile = nextTile;

		centreX += movementSpeed * dirX;
		centreY += movementSpeed * dirY;
		
		shape->move(movementSpeed * dirX, movementSpeed * dirY);
		
		prevDirX = dirX;
		prevDirY = dirY;

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