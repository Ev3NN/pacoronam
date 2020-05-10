#include <iostream>

#include "Character.hpp"
#include "utils.hpp"

/* --- PROTECTED FUNCTIONS --- */

/* Tile* Character::find_next_tile() {

	c_int iNextTile = centreY / CELL_SIZE;
	c_int jNextTile = centreX / CELL_SIZE;

	std::cout << "i j: " << iNextTile << " " << jNextTile << "\n";

	// Check new i,j: Out of bounds => Tunnel
	if(out_of_grid(iNextTile, jNextTile)) {
		std::cout << "OUT OF BOUNDS\n";
		return nullptr;
	}

	return grid->get_tile_at(iNextTile, jNextTile);
}
 */

Tile* Character::find_next_tile() {
	return grid->get_tile_at(aboveTile->rows + this->dirY, aboveTile->cols + this->dirX);
}

void Character::init_player(Grid* grid, c_float movementSpeed, c_float centreX, c_float centreY) {
	this->movementSpeed = movementSpeed;
	this->centreX = centreX;
	this->centreY = centreY;
	this->dirX = this->dirY = 0;
	this->grid = grid;
	this->aboveTile = nullptr;
}

void Character::init_monster(Grid* grid, c_string& name) {
	this->movementSpeed = 0.95f * REF_SPEED;
	this->dirX = this->dirY = 0;
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

void Character::handle_initial_move() {

	// Only horizontal movements can change this state (nullptr) 
	if(dirY != 0)
		return;

	else if(dirX == 1)
		aboveTile = grid->get_tile_at(26, 14);

	else if(dirX == -1)
		aboveTile = grid->get_tile_at(26, 13);

	centreX = centreX + movementSpeed * dirX;
	centreY = centreY + movementSpeed * dirY;
	shape->move(movementSpeed * dirX, movementSpeed * dirY);
}

bool Character::handle_wall_collisions(Tile* nextTile) {
	if(nextTile->is_wall()) {
		// std::cout << "There's a wall there, funny guy\n";

		// Right
		if(dirX == 1 && sf::Vector2f(centreX + movementSpeed, centreY).x >= aboveTile->get_tile_centre().x) {
			centreX = aboveTile->get_tile_centre().x;
			return true;
		}

		// Left
		if(dirX == -1 && sf::Vector2f(centreX - movementSpeed, centreY).x <= aboveTile->get_tile_centre().x) {
			centreX = aboveTile->get_tile_centre().x;
			return true;
		}

		// Down
		if(dirY == 1 && sf::Vector2f(centreX, centreY + movementSpeed).y >= aboveTile->get_tile_centre().y) {
			centreY = aboveTile->get_tile_centre().y;
			return true;
		}

		// Up
		if(dirY == -1 && sf::Vector2f(centreX, centreY - movementSpeed).y <= aboveTile->get_tile_centre().y) {
			centreY = aboveTile->get_tile_centre().y;
			return true;
		}


		// Update things
	}

	return false;
}

void Character::move(c_int dirX, c_int dirY) {
	c_int tmpDirX = this->dirX;
	c_int tmpDirY = this->dirY;

	this->dirX = dirX;
	this->dirY = dirY;

	// Special case: at the beginning of the game, the characters are positioned between two tiles
	// Hence the aboveTile = nullptr

	if(!aboveTile) {
		handle_initial_move();
		return;
	}
	
	// To implement later. See what's best for the underground implementation
	// Next tile is the underground
	if(in_tunnel(aboveTile->rows + dirY, aboveTile->cols + dirX)) {
		// Will block pacman as soon as he reaches the last tile		
		return;
	}

	Tile* nextTile = find_next_tile();


	if(handle_wall_collisions(nextTile)) {
		//centreX = aboveTile->get_tile_centre().x;
		//centreY = aboveTile->get_tile_centre().y;
		shape->setPosition(centreX, centreY);

		return;
	}


	// Pour valider le move, le centre de pac-man doit se trouver dans la case

	

	// problème à régler: Lorsque pac-man veut tourner (check dirX, dirY), on doit imposer 
	// que le centre de pac-man soit au centre de la tile

	if(dirX != tmpDirX && dirY != tmpDirY) {

		// Cannot move
		if(dirX != -1 && dirX != 1 && !cmp_float(centreX, aboveTile->get_tile_centre().x)) {
			// std::cout << "cannot move vert., not aligned";
			this->dirX = tmpDirX;
			this->dirY = tmpDirY;
			return;
		}
		else if(dirY != -1 && dirY != 1 && !cmp_float(centreY, aboveTile->get_tile_centre().y)) {
			// std::cout << "cannot move horiz., not aligned";
			this->dirX = tmpDirX;
			this->dirY = tmpDirY;
			return;
		}


	}

	c_float tmpCentreX = centreX + movementSpeed * dirX;
	c_float tmpCentreY = centreY + movementSpeed * dirY;

	

	/* if(nextTile->get_bounds().contains(sf::Vector2f(tmpCentreX, tmpCentreY))) {
		std::cout << "contains !!!!\n";
	}
	else {
		std::cout << "doesn't contain\n";
		std::cout << "Coord: !\n\t";
		std::cout << "x: " << nextTile->get_bounds().left / 28.f << "\n\t";
		std::cout << "y: " << nextTile->get_bounds().top / 28.f<< "\n";

		std::cout << "Coord centre: !\n\t";
		std::cout << "x: " << tmpCentreX << "\n\t";
		std::cout << "y: " << tmpCentreY << "\n";


	} */
		

	// Changement de case
	if(nextTile->get_bounds().contains(sf::Vector2f(tmpCentreX, tmpCentreY))) {
		// std::cout << "contains !\n";
		// std::cout << "x: " << nextTile->get_bounds().left << "\n";
		// std::cout << "y: " << nextTile->get_bounds().top << "\n";
		// std::cout << "x: " << nextTile->get_bounds().width << "\n";
		aboveTile = nextTile;
	}

	


	// SInon, on reste sur la même case tout en avancant visuellement. Les cas de collisions
	// avec les murs, monstres etc sont gérés au dessus

	centreX = tmpCentreX;
	centreY = tmpCentreY;
	shape->move(movementSpeed * dirX, movementSpeed * dirY);
	// std::cout << "tiletype: " << aboveTile->tileType << "\n";	



	/* else if(nextTile->tileType == TREAT_TILE) {

		// Check if there is a collision
		centreX = movementSpeed * dirX;
		centreY = movementSpeed * dirY;

		if(this->shape->getGlobalBounds().intersects(nextTile->food->shape.getGlobalBounds())) {
			std::cout << "hiiii\n";
		}
			

		// Remove the treat in grid->remainingFood

		// Increase score


		// Eat
		// Update things
	}

	else if(nextTile->tileType == PILL_TILE) {
		std::cout << "DONT MESS W/ ME DUDE\n";


		// Check if there is a collision

		// remove the pill in grid->remaningFood

		// Handle power pill
		// Update things
	}

	else if(nextTile->tileType == EMPTY_TILE) {
		std::cout << "Pfff, nothing to do. Not fun mate\n";

		// Update things though
	}

	else if(nextTile->tileType == DOOR_TILE) {
		std::cout << "U can't enter here. Bad guy only !\n";

		// Same as for walls. We need to detect it sooner

		return;
	}

	else {
		std::cout << "i might have forgotten something...\n";
	}

	shape->move(movementSpeed * dirX, movementSpeed * dirY); */


















	/* float tmpCentreX = centreX;
	float tmpCentreY = centreY;

	centreX += movementSpeed * dirX;
	centreY += movementSpeed * dirY;

	Tile* nextTile = find_next_tile();
	if(nextTile == nullptr)
		std::cout << "hi talk plz\n";

	// nextTile can be null if a character uses the underground tunnel


	if(!aboveTile) {
		std::cout << "First move\n";
		// Beginning of the game: pac-man is between two tiles			
		aboveTile = nextTile;
	}

	// Compare t and aboveTile
	if(!aboveTile->compare(nextTile)) {
		// Checking tile cases

		

		// Underground tunnet
		if(!nextTile) {
			std::cout << "Here's the tunnel boiii\n";
			// Update things
		}

		else if(nextTile->is_wall()) {
			std::cout << "There's a wall there, funny guy\n";
			// Update things

			// Not fully working. Collisions must be detected sooner
			// Otherwise, characters can move in both direction even if there is only one tile large
			centreX = tmpCentreX;
			centreY = tmpCentreY;

			return;
		}

		else if(nextTile->tileType == TREAT_TILE) {
			std::cout << "Miam miam\n";

			// Remove the treat in grid->remainingFood

			// Increase score


			// Eat
			// Update things
		}

		else if(nextTile->tileType == PILL_TILE) {
			std::cout << "DONT MESS W/ ME DUDE\n";

			// remove the pill in grid->remaningFood

			// Handle power pill
			// Update things
		}

		else if(nextTile->tileType == EMPTY_TILE) {
			std::cout << "Pfff, nothing to do. Not fun mate\n";

			// Update things though
		}

		else if(nextTile->tileType == DOOR_TILE) {
			std::cout << "U can't enter here. Bad guy only !\n";

			// Same as for walls. We need to detect it sooner
			centreX = tmpCentreX;
			centreY = tmpCentreY;

			return;
		}

		else {
			std::cout << "i might have forgotten something...\n";
		}

	}

	// Need to update aboveTile

	shape->move(movementSpeed * dirX, movementSpeed * dirY); */
}

void Character::render(sf::RenderTarget* target) {
	target->draw(*shape);
}