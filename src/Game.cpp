#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "constants.hpp"
#include "Game.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Game::init_window() {
	window = new sf::RenderWindow(sf::VideoMode(GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE), "PACoronam");
	window->setFramerateLimit(FPS);
	window->setKeyRepeatEnabled(false);
}

void Game::init_grid() {
	grid = new Grid();
}

void Game::init_player() {
	player = new Player(grid);
}

void Game::init_monsters() {
	Monster* blinky = new Monster(grid, player, "Blinky");
	Monster* pinky = new Monster(grid, player, "Pinky");
	Monster* inky = new Monster(grid, player, "Inky");
	Monster* clyde = new Monster(grid, player, "Clyde");

	monsters.insert(std::pair<string, Monster*> ("Blinky", blinky));
	monsters.insert(std::pair<string, Monster*> ("Pinky", pinky));
	monsters.insert(std::pair<string, Monster*> ("Inky", inky));
	monsters.insert(std::pair<string, Monster*> ("Clyde", clyde));
}

void Game::update_poll_events() {
	sf::Event e;
	
	while(window->pollEvent(e)) {
		if(e.type == sf::Event::Closed)
			window->close();
	}
}

void Game::update_input() {
	static string prevKey = "Reset";

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && prevKey.compare("Left")) {
		player->set_direction(-1, 0);
		prevKey = "Left";
	}
		

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && prevKey.compare("Up")) {
		player->set_direction(0, -1);
		prevKey = "Up";
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && prevKey.compare("Right")) {
		player->set_direction(1, 0);
		prevKey = "Right";
	}
		
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && prevKey.compare("Down")) {
		player->set_direction(0, 1);
		prevKey = "Down";
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && prevKey.compare("Reset")) {
		prevKey = "Reset";
		reset();
	}
}

void Game::update() {

	update_input();
	
	grid->update();
	player->update();
	
	for(auto &monster : monsters)
		monster.second->update();
		
}

void Game::reset() {

	grid->reset();

	player->reset(grid);

	for(auto &monster : monsters)
		monster.second->reset(grid, player, monster.first);
}

void Game::render() {
	window->clear();

	grid->render(window);
	player->render(window);
	
	for(auto &monster : monsters)
		monster.second->render(window);

	window->display();
}

/* --- PUBLIC FUNCTIONS --- */

// Constructor & Destructor
Game::Game() {
	init_window();
	init_grid();
	init_player();
	init_monsters();
}

Game::~Game() {
	delete window;
	delete grid;
	delete player;

	for(auto &monster : monsters)
		delete monster.second;
}

void Game::run() {
	while(window->isOpen()) {
		update_poll_events();

		update();

		render();
	}
}