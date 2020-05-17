#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

#include "constants.hpp"
#include "Game.hpp"

/* --- PRIVATE FUNCTIONS --- */

void Game::init_window() {
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE), "PACoronam");
	window->setFramerateLimit(FPS);
	window->setVerticalSyncEnabled(false);
	window->setKeyRepeatEnabled(false);
}

void Game::init_grid() {
	grid = std::make_shared<Grid>();
}

void Game::init_score() {
	score = std::make_shared<Score>();
}

void Game::init_player() {
	player = std::make_shared<Player>(grid, score);
}

void Game::init_monsters() {
	auto blinky = std::make_shared<Monster>(grid, player, score, "Blinky");
	auto pinky = std::make_shared<Monster>(grid, player, score, "Pinky");
	auto inky = std::make_shared<Monster>(grid, player, score, blinky, "Inky");
	auto clyde = std::make_shared<Monster>(grid, player, score, "Clyde");

	monsters.insert(std::pair<string, std::shared_ptr<Monster>> ("Blinky", blinky));
	monsters.insert(std::pair<string, std::shared_ptr<Monster>> ("Pinky", pinky));
	monsters.insert(std::pair<string, std::shared_ptr<Monster>> ("Inky", inky));
	monsters.insert(std::pair<string, std::shared_ptr<Monster>> ("Clyde", clyde));
}

void Game::init_text() {
	text = std::make_unique<Text>();
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
	if(player->get_above_tile()) {
		text->in_game_text();
	}
	
	
	// Blinky MUST be updated before Inky

	

	monsters["Blinky"]->update();
	monsters["Pinky"]->update();
	monsters["Inky"]->update();
	monsters["Clyde"]->update();

	if(player->get_above_tile())
		Monster::update_timer();
}

void Game::reset() {

	grid->reset();

	score->reset();

	text->reset();

	player->reset(grid);

	for(auto &monster : monsters)
		monster.second->reset(grid, player, monster.first);
}

void Game::render() {
	window->clear();

	grid->render(window.get());
	player->render(window.get());

	// Display the monsters in this order. Loop is not reliable
	monsters["Blinky"]->render(window.get());
	monsters["Pinky"]->render(window.get());
	monsters["Inky"]->render(window.get());
	monsters["Clyde"]->render(window.get());

	score->render(window.get());
	text->render(window.get());

	window->display();
}

/* --- PUBLIC FUNCTIONS --- */

// Constructor & Destructor
Game::Game() {
	init_window();
	init_grid();
	init_score();
	init_player();
	init_monsters();
	init_text();
}

void Game::run() {
	while(window->isOpen()) {
		update_poll_events();

		update();

		render();
	}
}