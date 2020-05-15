#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "Monster.hpp"
#include "utils.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Monster::init_variables(std::shared_ptr<Player> player, c_string name, std::shared_ptr<Monster> blinky) {
	this->name = name;
	this->player = player;
	this->blinky = blinky;
	
	if(!name.compare("Blinky"))
		mode = Mode::CHASE;
	else 
		mode = Mode::SCATTER;
		
	patternMode = mode;
}

void Monster::init_time(c_string& name) {
	if(!name.compare("Blinky")) {
		patternModeCooldown = 39;
		startCountdown = 0;
	}
	else
		patternModeCooldown = 9;

	startCountdown = 0;
	if(!name.compare("Inky"))
		startCountdown = 15;
	else if(!name.compare("Clyde"))
		startCountdown = 60;

	pillsCooldownSet = 0;
	panicCooldown = 0;
	timer = 0;
}

void Monster::init_shape(c_string& name) {

	c_uint pxSize = 1.6f * CELL_SIZE;

	shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(pxSize, pxSize));
	shape->setOrigin(pxSize / 2.f, pxSize / 2.f);

	if(!name.compare("Blinky")) {
		shape->setPosition(14 * CELL_SIZE, 14.5f * CELL_SIZE);
		shape->setFillColor(sf::Color::Red);
	}
		
	else if(!name.compare("Pinky")) {
		shape->setPosition(14 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color(250, 197, 246));
	}
		
	else if(!name.compare("Inky")) {
		shape->setPosition(12 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color::Cyan);
	}
	else if(!name.compare("Clyde")) {
		shape->setPosition(16 * CELL_SIZE, 17.5f * CELL_SIZE);
		shape->setFillColor(sf::Color(247, 187, 20));
	}
}

void Monster::init_target(c_string& name) {
	if(!name.compare("Blinky"))
		target = *(grid->get_tile_at(0, 25));
	else if(!name.compare("Pinky"))
		target = *(grid->get_tile_at(0, 2));
	else if(!name.compare("Inky"))
		target = *(grid->get_tile_at(35, 27));
	else if(!name.compare("Clyde"))
		target = *(grid->get_tile_at(35, 0));

	hasTarget = true;
}

void Monster::update_panic() {
	if(player->get_pills_eating() > pillsCooldownSet) {
		// std::cout << "Switching to PANIC MODE !\n";

		mode = Mode::PANIC;
		panicCooldown += 7;
		++pillsCooldownSet;
	}
}

bool Monster::can_leave_house() {
	if(!player->aboveTile)
		return false;

	if(!startCountdown /* add eaten food condition */)
		return true;

	return false;
}

void Monster::update_mode() {
	if(patternModeCooldown)
		return;

	if(patternMode == Mode::CHASE) {
		// std::cout << "Switching to SCATTER MODE !\n";

		patternModeCooldown = 9;
		patternMode = Mode::SCATTER;
	}
	else if(patternMode == Mode::SCATTER) {
		// std::cout << "Switching to CHASE MODE !\n";

		patternModeCooldown = 30;
		patternMode = Mode::CHASE;
	}
}

void Monster::update_start_countdown() {
	timer = ++timer % FPS;

	if(timer)
		return;

	if(startCountdown > 0)
		--startCountdown;	
}

void Monster::update_time() {
	timer = ++timer % FPS;

	if(timer)
		return;

	if(panicCooldown > 0)
		--panicCooldown;
	
	if(!panicCooldown) {
		mode = patternMode;
		player->set_pills_eating(0);
		pillsCooldownSet = 0;
	}

	else if(!(panicCooldown % 7)) {
		player->set_pills_eating((player->get_pills_eating()) - 1);
		--pillsCooldownSet;
	}

	--patternModeCooldown;

	// if(mode == Mode::PANIC)
	// 	std::cout << "PACNIC: " << panicCooldown << "\n";
	// else if(mode == Mode::SCATTER)
	// 	std::cout << "SCATTER: " << patternModeCooldown << "\n";
	// else if(mode == Mode::CHASE)
	// 	std::cout << "CHASE: " << patternModeCooldown << "\n";

}

void Monster::update_blinky_chase_target() {
	target = *(player->aboveTile);
}

void Monster::update_pinky_chase_target() {
	int targetRows = player->aboveTile->rows + 4 * player->prevDirY;
	int targetCols = player->aboveTile->cols + 4 * player->prevDirX;

	if(is_out_of_bounds(targetRows, targetCols))
		target = Tile(VOID_TILE, targetRows, targetCols);
	else
		target = *(grid->get_tile_at(targetRows, targetCols));
}

void Monster::update_inky_chase_target() {
	// Hardcoded for now, until our monsters move autom
	Tile blinkyTile = Tile(EMPTY_TILE, 14, 15);
	//Tile blinkyTile = *(blinky->aboveTile);
	Tile midTarget = *(player->find_next_tile(player->prevDirX, player->prevDirY));

	sf::Vector2f blinkyTileCentre = blinkyTile.get_tile_centre();
	sf::Vector2f midTargetCentre = midTarget.get_tile_centre();

	float maxX = std::max(blinkyTileCentre.x, midTargetCentre.x);
	float minX = std::min(blinkyTileCentre.y, midTargetCentre.y);
	float maxY = std::max(blinkyTileCentre.x, midTargetCentre.x);
	float minY = std::min(blinkyTileCentre.y, midTargetCentre.y);
	
	float diffX = maxX - minX;
	float diffY = maxY - minY;

	float targetX, targetY;

	if(blinkyTileCentre.x > midTargetCentre.x)
		targetX = blinkyTileCentre.x - 2 * diffX;
	else
		targetX = blinkyTileCentre.x + 2 * diffX;

	if(blinkyTileCentre.y > midTargetCentre.y)
		targetY = blinkyTileCentre.y - 2 * diffY;
	else
		targetY = blinkyTileCentre.y + 2 * diffY;

	int targetRows = targetY / GRID_ROWS;
	int targetCols = targetX / GRID_COLS;

	if(is_out_of_bounds(targetRows, targetCols))
		target = Tile(VOID_TILE, targetRows, targetCols);
	else
		target = *(grid->get_tile_at(targetRows, targetCols));
}

void Monster::update_clyde_chase_target() {
	sf::Vector2f playerCentre = sf::Vector2f(player->centreX, player->centreY);
	sf::Vector2f monsterCentre = sf::Vector2f(centreX, centreY);
	if(distance(playerCentre, monsterCentre) < 8 * CELL_SIZE)
		target = *(grid->get_tile_at(35, 0));
	else
		target = *(player->aboveTile);
}

void Monster::update_chase_target() {
	if(mode != Mode::CHASE)
		return;

	if(!name.compare("Blinky"))
		update_blinky_chase_target();

	else if(!name.compare("Pinky"))
		update_pinky_chase_target();
		
	else if(!name.compare("Inky"))
		update_inky_chase_target();

	else if(!name.compare("Clyde"))
		update_clyde_chase_target();
}

void Monster::update_scatter_target() {
	if(mode != Mode::SCATTER)
		return;

	if(!name.compare("Blinky"))
		target = *(grid->get_tile_at(0, 25));

	else if(!name.compare("Pinky"))
		target = *(grid->get_tile_at(0, 2));

	else if(!name.compare("Inky"))
		target = *(grid->get_tile_at(35, 27));

	else if(!name.compare("Clyde"))
		target = *(grid->get_tile_at(35, 0));
}

void Monster::update_target() {

	update_chase_target();

	update_scatter_target();

	if(mode == Mode::PANIC)
		hasTarget = false;
}

void Monster::move() {
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Monster::Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, 
				c_string& name) : Character(grid, name) {

	init_variables(player, name);
	init_time(name);
	init_target(name);
	init_shape(name);
}

Monster::Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, 
				std::shared_ptr<Monster> blinky, c_string& name) : Character(grid, name) {

	init_variables(player, name, blinky);
	init_time(name);
	init_target(name);
	init_shape(name);
}

void Monster::reset(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, c_string& name) {
	Character::init_variables(grid);
	init_monster(name);
	Monster::init_variables(player, name);
	init_shape(name);
}

void Monster::update() {
	// To test for only one monster
	if(name.compare("Clyde"))
		return;
		

	if(!player->aboveTile)
		return;

	// Inky and Clyde cannot leave the monster house right away
	if(!can_leave_house()) {
		update_start_countdown();
		//std::cout << "Remaning time before slaughter: " << startCountdown << "\n";
		return;
	}

	// Updates the current mode and starts a timer
	update_panic();
		
	// Updates the modes that must respect the pattern
	update_mode();

	// Updates the target in function of the current mode
	update_target();


	// std::cout << "TARGET: " << target.rows << " " << target.cols << "\n";

	// Routing
	// Move

	// Updates the timer, cooldowns, and countdowns: an iteration = 1/FPS = 16ms
	update_time();
}