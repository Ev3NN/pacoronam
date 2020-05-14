#include <SFML/Graphics.hpp>
#include <iostream>

#include "Monster.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Monster::init_variables(Player* player, c_string name) {
	this->name = name;
	this->player = player;
	
	if(!name.compare("Blinky")) {
		mode = Mode::CHASE;
		modeCooldown = 39;
	}
	else {
		mode = Mode::SCATTER;
		modeCooldown = 9;
	}
	
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
		panicCooldown += 7;
		++pillsCooldownSet;
	}
}

void Monster::update_mode() {
	if(modeCooldown)
		return;

	if(mode == Mode::CHASE) {
		modeCooldown = 9;
		mode = Mode::SCATTER;
	}
	else if(mode == Mode::SCATTER) {
		modeCooldown = 30;
		mode = Mode::CHASE;
	}
}

void Monster::update_time() {
	timer = ++timer % FPS;

	if(timer)
		return;

	if(panicCooldown > 0)
		--panicCooldown;
	
	if(!panicCooldown) {
		player->set_pills_eating(0);
		pillsCooldownSet = 0;
	}

	else if(!(panicCooldown % 7)) {
		player->set_pills_eating((player->get_pills_eating()) - 1);
		--pillsCooldownSet;
	}

	--modeCooldown;
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
	// To debug for only one monster
	if(name.compare("Blinky"))
		return;

	update_panic();
	
	
	// Update mode
	update_mode();

	// Get new target
	// Routing
	// Move
	// Panic mode
	// Update timer/cooldown

	update_time();

}