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

void Character::move(c_int dirX, c_int dirY) {
	c_int tmpDirX = this->dirX;
	c_int tmpDirY = this->dirY;

	this->dirX = dirX;
	this->dirY = dirY;

	// Special case: at the beginning of the game, the characters are positioned between two tiles
	// Hence the aboveTile = nullptr
	if(!aboveTile) {

		// Only horizontal movements can change this state (nullptr) 
		if(dirX == 1)
			aboveTile = grid->get_tile_at(26, 14);

		else if(dirX == -1)
			aboveTile = grid->get_tile_at(26, 13);
		else
			return;

		centreX = centreX + movementSpeed * dirX;
		centreY = centreY + movementSpeed * dirY;
		shape->move(movementSpeed * dirX, movementSpeed * dirY);

	/* 	std::cout << "tiletype: " << aboveTile->tileType << "\n";	

		std::cout << "Coord centre: !\n\t";
		std::cout << "x: " << centreX << "\n\t";
		std::cout << "y: " << centreY << "\n"; */

		return;
	}

	// To implement later. See what's best for the underground implementation
	if(in_tunnel(aboveTile->rows + dirY, aboveTile->cols + dirX)) {
		// std::cout << "yo\n";
	}

	Tile* tmpTile = find_next_tile();
	// std::cout << "Before check: tyletype = " << tmpTile->tileType << "\n";
	if(tmpTile->is_wall()) {
		// std::cout << "There's a wall there, funny guy\n";

		if(dirX == 1 && sf::Vector2f(centreX, centreY).x >= aboveTile->get_tile_centre().x) {
			centreX = aboveTile->get_tile_centre().x;
			shape->setPosition(centreX, centreY);
			// std::cout << "centre x: " << aboveTile->get_tile_centre().x;
			return;
		}
		if(dirX == -1 && sf::Vector2f(centreX, centreY).x <= aboveTile->get_tile_centre().x) {
			centreX = aboveTile->get_tile_centre().x;
			shape->setPosition(centreX, centreY);
			return;
		}
		if(dirY == 1 && sf::Vector2f(centreX, centreY).y >= aboveTile->get_tile_centre().y) {
			centreY = aboveTile->get_tile_centre().y;
			shape->setPosition(centreX, centreY);
			return;
		}
		if(dirY == -1 && sf::Vector2f(centreX, centreY).y <= aboveTile->get_tile_centre().y) {
			centreY = aboveTile->get_tile_centre().y;
			shape->setPosition(centreX, centreY);
			return;
		}
		// Update things

		// Not fully working. Collisions must be detected sooner
		// Otherwise, characters can move in both direction even if there is only one tile large

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

	

	/* if(tmpTile->get_bounds().contains(sf::Vector2f(tmpCentreX, tmpCentreY))) {
		std::cout << "contains !!!!\n";
	}
	else {
		std::cout << "doesn't contain\n";
		std::cout << "Coord: !\n\t";
		std::cout << "x: " << tmpTile->get_bounds().left / 28.f << "\n\t";
		std::cout << "y: " << tmpTile->get_bounds().top / 28.f<< "\n";

		std::cout << "Coord centre: !\n\t";
		std::cout << "x: " << tmpCentreX << "\n\t";
		std::cout << "y: " << tmpCentreY << "\n";


	} */
		

	// Changement de case
	if(tmpTile->get_bounds().contains(sf::Vector2f(tmpCentreX, tmpCentreY))) {
		// std::cout << "contains !\n";
		// std::cout << "x: " << tmpTile->get_bounds().left << "\n";
		// std::cout << "y: " << tmpTile->get_bounds().top << "\n";
		// std::cout << "x: " << tmpTile->get_bounds().width << "\n";
		aboveTile = tmpTile;
	}

	


	// SInon, on reste sur la même case tout en avancant visuellement. Les cas de collisions
	// avec les murs, monstres etc sont gérés au dessus

	centreX = tmpCentreX;
	centreY = tmpCentreY;
	shape->move(movementSpeed * dirX, movementSpeed * dirY);
	// std::cout << "tiletype: " << aboveTile->tileType << "\n";	



	/* else if(tmpTile->tileType == TREAT_TILE) {

		// Check if there is a collision
		centreX = movementSpeed * dirX;
		centreY = movementSpeed * dirY;

		if(this->shape->getGlobalBounds().intersects(tmpTile->food->shape.getGlobalBounds())) {
			std::cout << "hiiii\n";
		}
			

		// Remove the treat in grid->remainingFood

		// Increase score


		// Eat
		// Update things
	}

	else if(tmpTile->tileType == PILL_TILE) {
		std::cout << "DONT MESS W/ ME DUDE\n";


		// Check if there is a collision

		// remove the pill in grid->remaningFood

		// Handle power pill
		// Update things
	}

	else if(tmpTile->tileType == EMPTY_TILE) {
		std::cout << "Pfff, nothing to do. Not fun mate\n";

		// Update things though
	}

	else if(tmpTile->tileType == DOOR_TILE) {
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

	Tile* tmpTile = find_next_tile();
	if(tmpTile == nullptr)
		std::cout << "hi talk plz\n";

	// tmpTile can be null if a character uses the underground tunnel


	if(!aboveTile) {
		std::cout << "First move\n";
		// Beginning of the game: pac-man is between two tiles			
		aboveTile = tmpTile;
	}

	// Compare t and aboveTile
	if(!aboveTile->compare(tmpTile)) {
		// Checking tile cases

		

		// Underground tunnet
		if(!tmpTile) {
			std::cout << "Here's the tunnel boiii\n";
			// Update things
		}

		else if(tmpTile->is_wall()) {
			std::cout << "There's a wall there, funny guy\n";
			// Update things

			// Not fully working. Collisions must be detected sooner
			// Otherwise, characters can move in both direction even if there is only one tile large
			centreX = tmpCentreX;
			centreY = tmpCentreY;

			return;
		}

		else if(tmpTile->tileType == TREAT_TILE) {
			std::cout << "Miam miam\n";

			// Remove the treat in grid->remainingFood

			// Increase score


			// Eat
			// Update things
		}

		else if(tmpTile->tileType == PILL_TILE) {
			std::cout << "DONT MESS W/ ME DUDE\n";

			// remove the pill in grid->remaningFood

			// Handle power pill
			// Update things
		}

		else if(tmpTile->tileType == EMPTY_TILE) {
			std::cout << "Pfff, nothing to do. Not fun mate\n";

			// Update things though
		}

		else if(tmpTile->tileType == DOOR_TILE) {
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