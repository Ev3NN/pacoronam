#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "constants.hpp"
#include "Game.hpp"

// Private functions
void Game::init_window() {
	this->window = new sf::RenderWindow(sf::VideoMode(GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE), "PACoronam");
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Game::init_grid() {
	grid = new Grid();
}

void Game::init_player() {
	player = new Player();
}

void Game::init_monsters() {
	Monster blinky("Blinky");
	Monster pinky("Pinky");
	Monster inky("Inky");
	Monster clyde("Clyde");

	monsters.insert(std::pair<std::string, Monster> ("Blinky", blinky));
	monsters.insert(std::pair<std::string, Monster> ("Pinky", blinky));
	monsters.insert(std::pair<std::string, Monster> ("Inky", blinky));
	monsters.insert(std::pair<std::string, Monster> ("Clyde", blinky));
}

// Public functions

// Constructor
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

	while(this->window->pollEvent(e)) {

		switch(e.type) {
			case sf::Event::Closed:
				window->close();
				break;

			default:
				break;
		}
	}
}

void Game::update_input() {

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		std::cout << "Left.\n";

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		std::cout << "Up.\n";

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		std::cout << "Right.\n";
		
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		std::cout << "Down.\n";
}

void Game::update() {

	this->update_input();
	grid->update();
	player->update();
	
	for(auto &monster : monsters)
		monster.second.update();
}

void Game::render() {
	window->clear();

	grid->render(window);
	player->render(window);

	for(auto &monster : monsters)
		monster.second.render(window);

	window->display();
}