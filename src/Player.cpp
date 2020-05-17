#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.hpp"
#include "Monster.hpp"
#include "constants.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Player::init_variables() {
	takeCorner = isCornering = false;
	digestCooldown = 0;
	isExtendingPill = true;
}

void Player::init_shape() {

	float radius = 0.8f * CELL_SIZE;

	shape = std::make_unique<sf::CircleShape>(radius);
	
	shape->setFillColor(sf::Color::Yellow);
	shape->setOrigin(radius, radius);
	shape->setPosition(14 * CELL_SIZE, 26.5f * CELL_SIZE);	
}

void Player::init_score(std::shared_ptr<Score> score) {
	this->score = score;
}

bool Player::is_digesting() {
	return digestCooldown > 0;
}

void Player::update_digestion_cooldown() {

	if(digestCooldown > 0)
		// 1/FPS = 16ms. The duration of an iteration in the main loop. Our cheap timer
		digestCooldown -= 1;
	else
		digestCooldown = 0;
}

bool Player::handle_initial_move() {
	// 'aboveTile' is nullptr at the beginning of the game
	if(aboveTile)
		return false;

	// Only horizontal movements can change this state (nullptr)
	if(nextDirX == 1)
		aboveTile = grid->get_tile_at(26, 14);

	else if(nextDirX == -1)
		aboveTile = grid->get_tile_at(26, 13);

	else
		return true;

	centreX += movementSpeed * nextDirX;
	centreY += movementSpeed * nextDirY;

	shape->move(movementSpeed * nextDirX, movementSpeed * nextDirY);

	movementDirX = nextDirX;
	movementDirY = nextDirY;

	return true;
}

bool Player::handle_blocking_wall() {
	if(is_turning())
		return false;

	// Can be put in a fonction imo

	// Right
	if(nextDirX == 1 && (centreX + movementSpeed) >= aboveTile->get_tile_centre().x) 
		centreX = aboveTile->get_tile_centre().x;
	// Left
	else if(nextDirX == -1 && (centreX - movementSpeed) <= aboveTile->get_tile_centre().x)
		centreX = aboveTile->get_tile_centre().x;		
	// Down
	else if(nextDirY == 1 && (centreY + movementSpeed) >= aboveTile->get_tile_centre().y) 
		centreY = aboveTile->get_tile_centre().y;
	//Up
	else if(nextDirY == -1 && (centreY - movementSpeed) <= aboveTile->get_tile_centre().y)
		centreY = aboveTile->get_tile_centre().y;
	else {
		centreX += movementSpeed * nextDirX;
		centreY += movementSpeed * nextDirY;

		shape->move(movementSpeed * nextDirX, movementSpeed * nextDirY);

		return true;
	}

	shape->setPosition(centreX, centreY);


	// We cannot have prevDir (0, 0), otherwise Pinky cannot
	// update his target properly in CHASE MODE
	//movementDirX = movementDirY;nextDirX = nextDirY = 0;

	nextDirX = nextDirY = 0;

	return true;
}

bool Player::handle_non_blocking_wall() {
	if(!is_turning())
		return false;

	auto nextTile = std::make_shared<Tile>(find_next_tile(movementDirX, movementDirY));
	// Handles cases where the player changes the direction but at the end of the hall, 
	//there is also a wall in that new direction
	if(nextTile.get()->is_restricted_area(canEnterMonsterHouse)) {
		// The situation becomes equivalent to moving until there is a wall in the direction of movement
		nextDirX = movementDirX;
		nextDirY = movementDirY;

		takeCorner = false;

		if(handle_blocking_wall())
			return true;
	}

	// Il n'y a pas de wall dans le sens de movement

	takeCorner = true;

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float nextCentreY = centreY + movementSpeed * movementDirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

	if(nextTile->get_bounds().contains(nextCentre)) {
		// std::cout << "Rentre dans la cond contains\n";
		aboveTile = nextTile;

		auto checkTile = find_next_tile(nextDirX, nextDirY).get();
		if(!checkTile->is_restricted_area(canEnterMonsterHouse)) {
			// std::cout << "Rentre dans le restricted area\n";

			if(movementDirX == -1) {
				centreX = aboveTile->get_bounds().left + CELL_SIZE;
				centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
			}
			else if(movementDirX == 1) {
				centreX = aboveTile->get_bounds().left;
				centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
			}
			else if(movementDirY == 1) {
				centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
				centreY = aboveTile->get_bounds().top;
			}
			else if(movementDirY == -1) {
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

	shape->move(movementSpeed * movementDirX, movementSpeed * movementDirY);

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

bool Player::handle_cornering() {
	if(!takeCorner)
		return false;

	isCornering = true;

	int cornernextDirX = movementDirX + nextDirX;
	int cornernextDirY = movementDirY + nextDirY;

	float nextCentreX = centreX + movementSpeed * cornernextDirX;
	float nextCentreY = centreY + movementSpeed * cornernextDirY;
	sf::Vector2f nextCentre = sf::Vector2f(nextCentreX, nextCentreY);

	auto nextTile = std::make_shared<Tile>(find_next_tile(nextDirX, nextDirY));
	if(nextTile.get()->get_bounds().contains(nextCentre)) {
		aboveTile = nextTile;

		// Centrer
		if(nextDirX == -1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE;
			centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(nextDirX == 1) {
			centreX = aboveTile->get_bounds().left;
			centreY = aboveTile->get_bounds().top + CELL_SIZE / 2.f;
		}
		else if(nextDirY == -1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = aboveTile->get_bounds().top + CELL_SIZE;
		}
		else if(nextDirY == 1) {
			centreX = aboveTile->get_bounds().left + CELL_SIZE / 2.f;
			centreY = aboveTile->get_bounds().top;
		}

		shape->setPosition(centreX, centreY);

		movementDirX = nextDirX;
		movementDirY = nextDirY;

		takeCorner = false;
		isCornering = false;

		return true;
	}

	centreX += movementSpeed * cornernextDirX;
	centreY += movementSpeed * cornernextDirY;


	shape->move(movementSpeed * cornernextDirX, movementSpeed * cornernextDirY);

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

void Player::handle_treat() {
	score->food_eat();
}

void Player::handle_pill() {
	score->pill_eat();
	Monster::set_panic_cooldown();
}

void Player::eat_food() {
	if(!aboveTile)
		return;

	if(aboveTile->tileType == TREAT_TILE) {
		handle_treat();
	}
	else if(aboveTile->tileType == PILL_TILE)
		handle_pill();
	else
		return;

	grid->remove_food(aboveTile->rows, aboveTile->cols);
	aboveTile = grid->get_tile_at(aboveTile->rows, aboveTile->cols);

	digestCooldown = 2;
}

void Player::move() {
	// When pac-man
		
	if(is_motionless())
		return;

	// Digesting takes a whole frame when Pac-Man cannot move
	if(is_digesting()) {
		//std::cout << "digesting ?\n";
		return;
	}
		
	// Special case: Pac-Man begins between two tiles
	if(handle_initial_move()) {
		return;
	}
		
	

	// If the next tile is in the tunnel
	if(handle_tunnel())
		return;

	if(handle_wall())
		return;

	if(handle_turn())
		return;

	else {
		auto nextTile = std::make_shared<Tile>(find_next_tile(nextDirX, nextDirY));
		float nextCentreX = centreX + movementSpeed * nextDirX;
		float nextCentreY = centreY + movementSpeed * nextDirY;

		if(nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY)))
			aboveTile = nextTile;

		centreX += movementSpeed * nextDirX;
		centreY += movementSpeed * nextDirY;
		
		shape->move(movementSpeed * nextDirX, movementSpeed * nextDirY);
		
		movementDirX = nextDirX;
		movementDirY = nextDirY;
	}
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Player::Player(std::shared_ptr<Grid> grid, std::shared_ptr<Score> score) : Character(grid) {
	init_variables();
	init_shape();
	init_score(score);
}

void Player::set_extending(c_bool isExtendingPill) {
	this->isExtendingPill = isExtendingPill;
}

void Player::set_direction(c_int nextDirX, c_int nextDirY) {
	if(is_under_tunnel() && nextDirX != movementDirX && nextDirY == movementDirY)
		return;

	else if(reaching_tunnel() && nextDirX != movementDirX && nextDirY == movementDirY) {
		this->movementDirX = this->nextDirX = nextDirX;
		this->movementDirY = this->nextDirY = nextDirY;
	}
	else if(moving_away_from_tunnel() && nextDirX != movementDirX && nextDirY == movementDirY) {
		this->movementDirX = this->nextDirX = nextDirX;
		this->movementDirY = this->nextDirY = nextDirY;
	}

	if(isCornering)
		return;

	if(is_changing_direction())
		takeCorner = false;

	this->nextDirX = nextDirX;
	this->nextDirY = nextDirY;
}

Tile* Player::get_above_tile() {
	return aboveTile.get();
}

void Player::reset(std::shared_ptr<Grid> grid) {
	Character::init_variables(grid);
	init_player();
	Player::init_variables();
	init_shape();
}

void Player::update() {
	
	move();
	eat_food();

	// std::cout << "In eat food - (4, 2): " << grid->get_tile_at(26, 12)->tileType << "\n";
	// if(aboveTile) {

	// 	aboveTile = grid->get_tile_at(26, 12);
	// 	std::cout << "Abovetile: tiletype: " << aboveTile->tileType << "\n";
	// }
		
	update_digestion_cooldown();
}