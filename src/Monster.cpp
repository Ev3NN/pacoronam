#include <SFML/Graphics.hpp>
#include <iostream>

#include "Monster.hpp"
#include "utils.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Monster::init_variables(Player* player, c_string name) {
	this->name = name;
	this->player = player;
	
	if(!name.compare("Blinky")) {
		mode = Mode::CHASE;
		patternModeCooldown = 39;
	}
	else {
		mode = Mode::SCATTER;
		patternModeCooldown = 9;
	}
	
	patternMode = mode;
	pillsCooldownSet = 0;
	panicCooldown = 0;
	timer = 0;
	target = nullptr;
}

void Monster::init_shape(c_string& name) {

	c_uint pxSize = 1.6f * CELL_SIZE;

	shape = new sf::RectangleShape(sf::Vector2f(pxSize, pxSize));
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

void Monster::update_panic() {
	if(player->get_pills_eating() > pillsCooldownSet) {
		mode = Mode::PANIC;
		panicCooldown += 7;
		++pillsCooldownSet;
	}
}

void Monster::update_mode() {
	if(patternModeCooldown)
		return;

	if(patternMode == Mode::CHASE) {
		patternModeCooldown = 9;
		patternMode = Mode::SCATTER;
	}
	else if(patternMode == Mode::SCATTER) {
		patternModeCooldown = 30;
		patternMode = Mode::CHASE;
	}
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
}

void Monster::update_chase_target() {
	if(mode != Mode::SCATTER)
		return;

	if(!name.compare("Blinky"))
		target = player->aboveTile;
	else if(!name.compare("Pinky"))
		target = player->find_next_tile(4 * player->prevDirX, 4 * player->prevDirY);
	else if(!name.compare("Inky"))
		target = nullptr;
	else if(!name.compare("Clyde")) {
		sf::Vector2f playerCentre = sf::Vector2f(player->centreX, player->centreY);
		sf::Vector2f monsterCentre = sf::Vector2f(centreX, centreY);
		if(distance(playerCentre, monsterCentre) < 8 * CELL_SIZE)
			target = grid->get_tile_at(35, 0);
		else
			target = player->aboveTile;
	}


}

void Monster::update_scatter_target() {
	if(mode != Mode::SCATTER)
		return;

	if(!name.compare("Blinky"))
		target = grid->get_tile_at(0, 25);
	else if(!name.compare("Pinky"))
		target = grid->get_tile_at(0, 2);
	else if(!name.compare("Inky"))
		target = grid->get_tile_at(35, 27);
	else if(!name.compare("Clyde"))
		target = grid->get_tile_at(35, 0);
}

void Monster::update_target() {

	update_chase_target();

	update_scatter_target();

	if(mode == Mode::PANIC)
		target = nullptr;
}

void Monster::move() {
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Monster::Monster(Grid* grid, Player* player, c_string& name) : Character(grid, name) {
	init_variables(player, name);
	init_shape(name);
}


Monster::~Monster() {
	delete shape;
}

void Monster::reset(Grid* grid, Player* player, c_string& name) {
	Character::init_variables(grid);
	init_monster(name);
	Monster::init_variables(player, name);
	init_shape(name);
}

void Monster::update() {
	if(!player->aboveTile)
		return;

	// To debug for only one monster
	if(name.compare("Blinky"))
		return;

	update_panic();
		
	// Update mode
	update_mode();

	update_target();


	// Seems working
	// std::cout << "Current mode: " << static_cast<std::underlying_type<Mode>::type>(mode) << "\n";
	// if(mode == Mode::PANIC)
	// 	std::cout << "Panic cooldown: " << panicCooldown << "\n";
	// else
	// 	std::cout << "Pattern cooldown: " << patternModeCooldown << "\n";


	// Get new target
	// Routing
	// Move
	// Panic mode
	// Update timer/cooldown

	update_time();

}