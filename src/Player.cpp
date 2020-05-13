#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.hpp"
#include "constants.hpp"

/* --- PRIVATE FUNCTIONS --- */
void Player::init_variables() {
	takeCorner = isCornering = false;
	digestCooldown = 0;
}

void Player::init_shape() {

	float radius = 0.8f * CELL_SIZE;

	shape = new sf::CircleShape(radius);
	
	shape->setFillColor(sf::Color::Yellow);
	shape->setOrigin(radius, radius);
	shape->setPosition(14 * CELL_SIZE, 26.5f * CELL_SIZE);	
}


bool Player::is_digesting() {
	return digestCooldown > 0;
}

bool Player::handle_initial_move() {
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

bool Player::handle_blocking_wall() {
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

bool Player::handle_non_blocking_wall() {
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

bool Player::handle_wall() {
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

void Player::handle_treat() {

}

void Player::handle_pill() {

}

void Player::eat_food() {
	if(!aboveTile)
		return;

	if(aboveTile->tileType == TREAT_TILE)
		handle_treat();

	else if(aboveTile->tileType == PILL_TILE)
		handle_pill();
	else
		return;

	grid->remove_food(aboveTile->rows, aboveTile->cols);
	digestCooldown = 32;
}

bool Player::handle_cornering() {
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

bool Player::handle_turn() {
	if(!is_turning()) 
		return false;
		
	// Cornering case
	if(handle_cornering())
		return true;
		
	// RIght angle case
	handle_right_angle();

	return true;
}

void Player::move() {
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

}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Player::Player(Grid* grid) : Character(grid) {
	init_variables();
	init_shape();
}


Player::~Player() {
	delete shape;
}




void Player::set_direction(c_int dirX, c_int dirY) {
	if(is_under_tunnel() && dirX != prevDirX && dirY == prevDirY) {
		// Useless ?
		// this->dirX = prevDirX;
		// this->dirY = prevDirY;

		return;
	}
	else if(reaching_tunnel() && dirX != prevDirX && dirY == prevDirY) {
		this->prevDirX = this->dirX = dirX;
		this->prevDirY = this->dirY = dirY;
	}
	else if(moving_away_from_tunnel() && dirX != prevDirX && dirY == prevDirY) {
		this->prevDirX = this->dirX = dirX;
		this->prevDirY = this->dirY = dirY;
	}

	if(isCornering)
		return;

	if(is_changing_direction())
		takeCorner = false;

	this->dirX = dirX;
	this->dirY = dirY;
}




















void Player::update_digestion_cooldown() {
	if(digestCooldown > 0)
		// 1/FPS = 16ms. The duration of an iteration in the main loop. Our cheap timer
		digestCooldown -= 16;
	else
		digestCooldown = 0;
}

void Player::update() {
	
	move();
	eat_food();

	update_digestion_cooldown();
}