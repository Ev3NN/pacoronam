#include <iostream>

#include "Character.hpp"
#include "utils.hpp"

/* --- PROTECTED FUNCTIONS --- */

void Character::init_variables(Grid* grid) {
	this->grid = grid;
	this->aboveTile = nullptr;
	this->dirX = this->dirY = this->prevDirX = this->prevDirY = 0;
}

void Character::init_player() {
	this->movementSpeed = REF_SPEED;
	this->centreX = 14.f * CELL_SIZE;
	this->centreY = 26.5f * CELL_SIZE;
	this->isMonsterHouseOpen = false;
}

void Character::init_monster(c_string& name) {
	this->movementSpeed = 0.95f * REF_SPEED;
	this->isMonsterHouseOpen = true;
	
	if(!name.compare("Blinky")) {
		centreX = 14 * CELL_SIZE;
		centreY = 14.5f * CELL_SIZE;
		this->isMonsterHouseOpen = false;
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

Tile* Character::find_next_tile(c_int& dirX, c_int& dirY) {
	return grid->get_tile_at(aboveTile->rows + dirY, aboveTile->cols + dirX);
}

bool Character::is_motionless() {
	return dirX == 0 && dirY == 0;
}

bool Character::is_turning() {
	// is_motionless needed ?
	return dirX != prevDirX && dirY != prevDirY && !is_motionless();
}

bool Character::is_changing_direction() {
	return (dirX != prevDirX || dirY != prevDirY) && !is_motionless();
}

bool Character::is_changing_orientation() {
	return (dirX == prevDirX && dirY != prevDirY) || (dirX != prevDirX && dirY == prevDirY);
}

bool Character::predict_wall_collision() {
	Tile* nextTile = find_next_tile(dirX, dirY);

	return nextTile->is_restricted_area(isMonsterHouseOpen);
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

bool Character::is_under_tunnel() {
	return centreX <= 0 || centreX >= GRID_COLS * CELL_SIZE;
}

bool Character::reaching_tunnel() {
	if(centreX <= CELL_SIZE && centreX >= 0 && prevDirX == -1)
		return true;
	if(centreX >= (GRID_COLS - 1) * CELL_SIZE && centreX <= GRID_COLS * CELL_SIZE && prevDirX == 1)
		return true;

	return false;
}

bool Character::moving_away_from_tunnel() {
	if(centreX <= CELL_SIZE && centreX >= 0 && prevDirX == 1)
		return true;
	if(centreX >= (GRID_COLS - 1) * CELL_SIZE && centreX <= GRID_COLS * CELL_SIZE && prevDirX == -1)
		return true;

	return false;
}

bool Character::handle_tunnel() {

	if(!reaching_tunnel() && !is_under_tunnel())
		return false;

	float nextCentreX = centreX + movementSpeed * prevDirX;

	if(nextCentreX <= - (int) CELL_SIZE && prevDirX == -1) {
		aboveTile = grid->get_tile_at(aboveTile->rows, GRID_COLS - 1);
		centreX = (GRID_COLS + 1) * CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}
	else if(nextCentreX >= (GRID_COLS + 1) * CELL_SIZE && prevDirX == 1) {
		aboveTile = grid->get_tile_at(aboveTile->rows, 0);
		centreX = - (int) CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}
	else {
		centreX = nextCentreX;
		shape->move(movementSpeed * prevDirX, 0);

		return true;
	}
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Character::Character(Grid* grid) {
	init_variables(grid);
	init_player();
}

Character::Character(Grid* grid, c_string& name) {
	init_variables(grid);
	init_monster(name);
}

Character::~Character() {}

void Character::render(sf::RenderTarget* target) {

	target->draw(*shape);
}