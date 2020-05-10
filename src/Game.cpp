#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "constants.hpp"
#include "Game.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Game::init_window() {
	window = new sf::RenderWindow(sf::VideoMode(GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE), "PACoronam");
	window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(false);
	window->setKeyRepeatEnabled(false);
}

void Game::init_grid() {
	grid = new Grid();
}

void Game::init_player() {
	player = new Player(grid);
}

void Game::init_monsters() {
	Monster* blinky = new Monster(grid, "Blinky");
	Monster* pinky = new Monster(grid, "Pinky");
	Monster* inky = new Monster(grid, "Inky");
	Monster* clyde = new Monster(grid, "Clyde");

	monsters.insert(std::pair<string, Monster*> ("Blinky", blinky));
	monsters.insert(std::pair<string, Monster*> ("Pinky", pinky));
	monsters.insert(std::pair<string, Monster*> ("Inky", inky));
	monsters.insert(std::pair<string, Monster*> ("Clyde", clyde));
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor
Game::Game() {
	this->init_window();
	this->init_grid();
	this->init_player();
	this->init_monsters();
}

Game::~Game() {
	delete window;
	delete grid;
	delete player;

	for(auto &monster : monsters)
		delete monster.second;
}

void Game::run() {

	while(this->window->isOpen()) {
		this->update_poll_events();

		this->update();

		this->render();
	}
}

void Game::update_poll_events() {
	sf::Event e;
	
	while(window->pollEvent(e)) {
		if(e.type == sf::Event::Closed)
			window->close();

		// Pacman must walk until there is a wall in front of him (or until another command is triggered)
		/* else if( e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Left)
			player->move(grid, -1.f, 0.f);

		else if( e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up)
			player->move(grid, 0.f, -1.f);

		else if( e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right)
			player->move(grid, 1.f, 0.f);
		
		else if( e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down)
			player->move(grid, 0.f, 1.f); */
	}
}


void Game::update_input() {

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		player->move(-1, 0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		player->move(0, -1);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		player->move(1, 0);
		
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		player->move(0, 1);
}


void Game::update() {

	this->update_input();
	grid->update();
	player->update();
	
	for(auto &monster : monsters)
		monster.second->update();
}

void Game::render() {
	window->clear();

	grid->render(window);
	player->render(window);
	
	for(auto &monster : monsters)
		monster.second->render(window);

	window->display();
}