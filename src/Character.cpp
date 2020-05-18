#include <iostream>

#include "Character.hpp"
#include "utils.hpp"

// static data member initialisation
bool Character::isInfected = false;
bool Character::isSick = false;
bool Character::isImmune = false;
uint Character::frameCount = 0;
uint Character::secCount = 0;
uint Character::panicCooldown = 0;

/* --- PROTECTED FUNCTIONS --- */

void Character::init_variables(std::shared_ptr<Grid> grid) {
	this->grid = grid;
	this->aboveTile = nullptr;
	this->nextDirX = this->nextDirY = this->movementDirX = this->movementDirY = 0;
}

void Character::init_player() {
	this->movementSpeed = 1.5f * REF_SPEED;
	this->centreX = 14.f * CELL_SIZE;
	this->centreY = 26.5f * CELL_SIZE;
	this->canEnterMonsterHouse = false;
}

void Character::init_monster(c_string& name) {
	this->movementSpeed = 0.95f * REF_SPEED;
	this->canEnterMonsterHouse = true;
	
	if(!name.compare("Blinky")) {
		centreX = 14 * CELL_SIZE;
		centreY = 14.5f * CELL_SIZE;
		this->canEnterMonsterHouse = false;
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

std::shared_ptr<Tile> Character::find_next_tile(c_int& dirX, c_int& dirY) {
	return grid->get_tile_at(aboveTile->rows + dirY, aboveTile->cols + dirX);
}

bool Character::is_motionless() {
	return nextDirX == 0 && nextDirY == 0;
}

bool Character::is_turning() {
	// is_motionless needed ?
	return nextDirX != movementDirX && nextDirY != movementDirY && !is_motionless();
}

bool Character::is_changing_direction() {
	return (nextDirX != movementDirX || nextDirY != movementDirY) && !is_motionless();
}

bool Character::is_changing_orientation() {
	return (nextDirX == movementDirX && nextDirY != movementDirY) || (nextDirX != movementDirX && nextDirY == movementDirY);
}

bool Character::predict_wall_collision() {
	Tile* nextTile = find_next_tile(nextDirX, nextDirY).get();

	return nextTile->is_restricted_area(canEnterMonsterHouse);
}

bool Character::is_right_angle_timing(c_float& centreX, c_float& centreY) {

	if(movementDirX == 1 && centreX <= aboveTile->get_bounds().left + CELL_SIZE / 2.f)
		return true;

	else if(movementDirX == -1 && centreX >= aboveTile->get_bounds().left + CELL_SIZE / 2.f)
		return true;

	else if(movementDirY == 1 && centreY <= aboveTile->get_bounds().top + CELL_SIZE / 2.f)
		return true;

	else if(movementDirY == -1 && centreY >= aboveTile->get_bounds().top + CELL_SIZE / 2.f)
		return true;

	else
		return false;
}

void Character::handle_right_angle() {
	if(!is_right_angle_timing(centreX, centreY)) {

		centreX += movementSpeed * movementDirX;
		centreY += movementSpeed * movementDirY;

		auto nextTile = std::make_shared<Tile>(find_next_tile(movementDirX, movementDirY));
		if(nextTile.get()->get_bounds().contains(sf::Vector2f(centreX, centreY)))
			aboveTile = nextTile;

		shape->move(movementSpeed * movementDirX, movementSpeed * movementDirY);
		
		return;
	}

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float nextCentreY = centreY + movementSpeed * movementDirY;

	// Si le prochain move amène le centre de pacman plus loin qu'au milieu de la tile
	if(!is_right_angle_timing(nextCentreX, nextCentreY)) {
		
		centreX = aboveTile->get_tile_centre().x;
		centreY = aboveTile->get_tile_centre().y;
		shape->setPosition(centreX, centreY);

		movementDirX = nextDirX;
		movementDirY = nextDirY;

		return;
	}

	centreX = nextCentreX;
	centreY = nextCentreY;
	shape->move(movementSpeed * movementDirX, movementSpeed * movementDirY);
}

bool Character::is_under_tunnel() {
	return centreX <= 0 || centreX >= GRID_COLS * CELL_SIZE;
}

bool Character::reaching_tunnel() {
	if(centreX <= CELL_SIZE && centreX >= 0 && movementDirX == -1)
		return true;
	if(centreX >= (GRID_COLS - 1) * CELL_SIZE && centreX <= GRID_COLS * CELL_SIZE && movementDirX == 1)
		return true;

	return false;
}

bool Character::moving_away_from_tunnel() {
	if(centreX <= CELL_SIZE && centreX >= 0 && movementDirX == 1)
		return true;
	if(centreX >= (GRID_COLS - 1) * CELL_SIZE && centreX <= GRID_COLS * CELL_SIZE && movementDirX == -1)
		return true;

	return false;
}

bool Character::handle_tunnel() {

	if(!reaching_tunnel() && !is_under_tunnel()) {
		return false;
	}

	float nextCentreX = centreX + movementSpeed * movementDirX;

	if(nextCentreX <= - (int) CELL_SIZE && movementDirX == -1) {
		aboveTile = grid->get_tile_at(aboveTile->rows, GRID_COLS - 1);
		centreX = (GRID_COLS + 1) * CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}
	else if(nextCentreX >= (GRID_COLS + 1) * CELL_SIZE && movementDirX == 1) {
		aboveTile = grid->get_tile_at(aboveTile->rows, 0);
		centreX = - (int) CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}
	else {
		centreX = nextCentreX;
		shape->move(movementSpeed * movementDirX, 0);

		return true;
	}
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Character::Character(std::shared_ptr<Grid> grid) {
	init_variables(grid);
	init_player();
}

Character::Character(std::shared_ptr<Grid> grid, c_string& name) {
	init_variables(grid);
	init_monster(name);
}

void Character::render(sf::RenderTarget* target) {

	target->draw(*shape);
}

void Character::get_infected() {
	if(isImmune)
		return;

	if(!isInfected && !isSick) {
		isInfected = true;
		frameCount = FPS;
		secCount = 20;
	}

	if(isSick && isInfected) {
		isInfected = false;
		frameCount = FPS;
		secCount = 10;
	}

}

void Character::update_timer() {
	if(isImmune || (!isInfected && !isSick))
		return;

	--frameCount;

	if(!frameCount) {
		frameCount = FPS;
		--secCount;
		if(!secCount) {
			if(!isSick) {
				isSick = true;
				return;
			}else {
				isSick = false;
				isImmune = true;
			}

		}
	}
	
}
