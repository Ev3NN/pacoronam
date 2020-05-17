#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <limits>

#include "Monster.hpp"
#include "utils.hpp"

// static data member initialisation
uint Monster::dsecTimer = 0;
uint Monster::secTimer = 0;
uint Monster::panicCooldown = 0;

/* --- PRIVATE FUNCTIONS --- */

void Monster::init_variables(std::shared_ptr<Player> pacman, std::shared_ptr<Score> score, 
							c_string name, std::shared_ptr<Monster> blinky) {
	this->name = name;
	this->pacman = pacman;
	// Only valid if monster name is "Inky"
	this->blinky = blinky;
	this->score = score;

	if(!name.compare("Blinky")) {
		mode = Mode::CHASE;
		canEnterMonsterHouse = false;
	}
		
	else 
		mode = Mode::SCATTER;
		
	patternMode = mode;
	isDead = false;
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

}

void Monster::init_directions(c_string& name) {

	if(!name.compare("Blinky")) {
		movementDirX = nextDirX = -1;
		movementDirY = nextDirY = 0;
	}
	else if(!name.compare("Pinky")) {
		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;
	}
	else if(!name.compare("Inky")) {
		movementDirX = nextDirX = 1;
		movementDirY = nextDirY = 0;
	}
	else if(!name.compare("Clyde")) {
		movementDirX = nextDirX = -1;
		movementDirY = nextDirY = 0;
	}

	mustCheckUTurn = false;
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
		target = grid->get_tile_at(0, 25);
	else if(!name.compare("Pinky"))
		target = grid->get_tile_at(0, 2);
	else if(!name.compare("Inky"))
		target = grid->get_tile_at(35, 27);
	else if(!name.compare("Clyde"))
		target = grid->get_tile_at(35, 0);

	hasTarget = true;
}

void Monster::update_panic() {
	if(isDead)
		return;

	if(!panicCooldown) {
		reset_monster_colour();
		pacman->set_extending(false);
		mode = patternMode;
		movementSpeed = 0.95f * REF_SPEED;
		score->end_panic();
	}
	else if(panicCooldown == 1 && dsecTimer == 5)
		shape->setFillColor(sf::Color::White);

	else if(panicCooldown == 7 && mode != Mode::PANIC) {
		mode = Mode::PANIC;
		movementSpeed = 0.77f * REF_SPEED;
		make_uturn();
	}
	else if(panicCooldown > 7) {
		pacman->set_extending(true);
		shape->setFillColor(sf::Color::Blue);
		movementSpeed = 0.77f * REF_SPEED;
	}
		

	else if(panicCooldown && aboveTile && shape->getFillColor() != sf::Color::White)
		shape->setFillColor(sf::Color::Blue);
}

bool Monster::can_leave_house() {

	if(!startCountdown /* add eaten food condition */)
		return true;

	return false;
}

void Monster::update_pattern_mode() {
	if(patternModeCooldown)
		return;

	if(patternMode == Mode::CHASE) {
		std::cout << "Switching to SCATTER MODE !\n";

		patternModeCooldown = 9;
		patternMode = Mode::SCATTER;
	}
	else if(patternMode == Mode::SCATTER) {
		std::cout << "Switching to CHASE MODE !\n";

		patternModeCooldown = 30;
		patternMode = Mode::CHASE;
	}
	else
		return;

	if(!isDead)
		make_uturn();
}

void Monster::update_start_timer() {
	if(secTimer)
		return;

	if(startCountdown > 0)
		--startCountdown;
}

void Monster::update_pattern_timer() {

	if(secTimer)
		return;

	if(patternModeCooldown > 0)
		--patternModeCooldown;
}

void Monster::reset_monster_colour() {
	if(!name.compare("Blinky")) 
		shape->setFillColor(sf::Color::Red);
		
	else if(!name.compare("Pinky"))
		shape->setFillColor(sf::Color(250, 197, 246));
		
	else if(!name.compare("Inky"))
		shape->setFillColor(sf::Color::Cyan);

	else if(!name.compare("Clyde"))
		shape->setFillColor(sf::Color(247, 187, 20));
}

void Monster::update_blinky_chase_target() {
	target = *(pacman->aboveTile);
}

void Monster::update_pinky_chase_target() {
	int targetRows = pacman->aboveTile->rows + 4 * pacman->movementDirY;
	int targetCols = pacman->aboveTile->cols + 4 * pacman->movementDirX;

	if(is_out_of_bounds(targetRows, targetCols))
		target = Tile(VOID_TILE, targetRows, targetCols);
	else
		target = *(grid->get_tile_at(targetRows, targetCols));
}

void Monster::update_inky_chase_target() {

	Tile blinkyTile;

	if(!blinky) {
		std::cout << name << "Blinky is null\n";
		Tile blinkyTile = *(grid->get_tile_at(14, 13));
	}
	else 
		blinkyTile = *(blinky->aboveTile);

	int midPointRows = pacman->aboveTile->rows + 2 * pacman->movementDirY;
	int midPointCols = pacman->aboveTile->cols + 2 * pacman->movementDirX;

	Tile midTarget;
	if(is_out_of_bounds(midPointRows, midPointCols))
		midTarget = Tile(VOID_TILE, midPointRows, midPointCols);
	else
		midTarget = *(pacman->find_next_tile(2 * pacman->movementDirX, 2 * pacman->movementDirY));

	sf::Vector2f blinkyTileCentre = blinkyTile.get_tile_centre();
	sf::Vector2f midTargetCentre = midTarget.get_tile_centre();

	float maxX = std::max(blinkyTileCentre.x, midTargetCentre.x);
	float minX = std::min(blinkyTileCentre.x, midTargetCentre.x);
	float maxY = std::max(blinkyTileCentre.y, midTargetCentre.y);
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

	int targetRows = targetY / CELL_SIZE;
	int targetCols = targetX / CELL_SIZE;

	if(is_out_of_bounds(targetRows, targetCols))
		target = Tile(VOID_TILE, targetRows, targetCols);
	else
		target = *(grid->get_tile_at(targetRows, targetCols));
}

void Monster::update_clyde_chase_target() {
	sf::Vector2f playerCentre = sf::Vector2f(pacman->centreX, pacman->centreY);
	sf::Vector2f monsterCentre = sf::Vector2f(centreX, centreY);
	
	if(distance(playerCentre, monsterCentre) < 8 * CELL_SIZE)
		target = *(grid->get_tile_at(35, 0));
	else
		target = *(pacman->aboveTile);
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
	if(isDead)
		return;

	if(mode == Mode::PANIC)
		hasTarget = false;

	hasTarget = true;
	update_chase_target();

	update_scatter_target();
}

std::vector<Tile> Monster::get_neighbours(Tile& refTile) {

	std::vector<Tile> neighbours;

	// Dead monsters can u-turn
	if(isDead && mustCheckUTurn) {
		neighbours.push_back(*(grid->get_tile_at(refTile.rows - 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols + 1)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows + 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols - 1)));
	}

	// Left	
	else if(movementDirX == -1) {
		neighbours.push_back(*(grid->get_tile_at(refTile.rows - 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows + 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols - 1)));
	}
	else if(movementDirX == 1) {
		neighbours.push_back(*(grid->get_tile_at(refTile.rows - 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols + 1)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows + 1, refTile.cols)));
	}
	else if(movementDirY == -1) {
		neighbours.push_back(*(grid->get_tile_at(refTile.rows - 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols + 1)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols - 1)));
	}
	else if(movementDirY == 1) {
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols + 1)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows + 1, refTile.cols)));
		neighbours.push_back(*(grid->get_tile_at(refTile.rows, refTile.cols - 1)));
	}

	for(auto itr = neighbours.begin(); itr != neighbours.end() ; ++itr)
		if(itr->is_restricted_area(canEnterMonsterHouse))
			neighbours.erase(itr--);

	return neighbours;
}

Tile Monster::get_closest_neighbour(Tile& refTile) {
	
	std::vector<Tile> neighbours = get_neighbours(refTile);

	float distMin = std::numeric_limits<float>::max();
	Tile closestNeighbour;

	if(isDead) {
			// std::cout << "reftile of : " << name <<  refTile.rows << " " << refTile.cols << "\n";
			// std::cout << "target: " << target.rows << " " << target.cols << "\n";
	}

	for(auto itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
		float currDist = distance(itr->get_tile_centre(), target.get_tile_centre());


		if(isDead) {
			// std::cout << "neighbour " << itr->rows << " " << itr->cols << "\n";
			// std::cout << "dist: " << currDist << "\n";
		}
		
		if(currDist < distMin) {
			distMin = currDist;
			closestNeighbour = *itr;
		}
	}

	// if(isDead) {
	// 	for(auto& neighbour : neighbours)
	// 		std::cout << "neighbours: " << neighbour.rows << " " << neighbour.cols << "\n";

	// 	std::cout << "Closest << " << closestNeighbour.rows << " " << closestNeighbour.cols << "\n\n";

	// }


	return closestNeighbour;
}

Tile Monster::get_random_neighbour(Tile& refTile) {
	std::vector<Tile> neighbours = get_neighbours(refTile);
	
	uint rdmIdx = rand() % neighbours.size();

	return neighbours.at(rdmIdx);
}

bool Monster:: is_routing_available() {
	if(!aboveTile) {
		// std::cout << "above tile null\n";
		return false;
	}
		

	c_uint rightLimit = (GRID_COLS - 2) * CELL_SIZE;
	if(aboveTile->rows == 17 && centreX <= 2 * CELL_SIZE && movementDirX == -1)
		return false;
	
	if(aboveTile->rows == 17 && centreX >= rightLimit && movementDirX == 1)
		return false;

	sf::Vector2f monsterCentre = sf::Vector2f(centreX, centreY);
	
	// Has not reached the centre
	if(!compare_point(aboveTile->get_tile_centre(), monsterCentre, 0.005f)) {
		// std::cout << "not centred\n";
		return false;
	}

	return true;
}

void Monster::handle_routing() {
	if(!is_routing_available()) {
		// std::cout << "Routing unavailable\n";
		//std::cout << "Cannot use routing now\n";
		return;
	}

	// Make random decisions after U-Turn
	if(mode == Mode::PANIC && !isDead)
		return;

	Tile closestNeighbour;
	if(mustCheckUTurn) {
		// std::cout << "Double routing !\n";
		closestNeighbour = get_closest_neighbour(*aboveTile);

		movementDirX = closestNeighbour.cols - aboveTile->cols;
		movementDirY = closestNeighbour.rows - aboveTile->rows;

		mustCheckUTurn = false;
	}
		
	Tile nextTile = *(find_next_tile(movementDirX, movementDirY));
	closestNeighbour = get_closest_neighbour(nextTile);

	nextDirX = closestNeighbour.cols - nextTile.cols;
	nextDirY = closestNeighbour.rows - nextTile.rows;

	// if(isDead) {
	// 	std::cout << "NewMovDir: " << movementDirX << " " << movementDirY << "\n";
	// 	std::cout << "newNextDir: " << nextDirX << " " << nextDirY << "\n";

	// }
}

void Monster::make_random_decision() {
	if(mode != Mode::PANIC || isDead)
		return;

	if(!is_routing_available())
		return;

	Tile closestNeighbour;
	if(mustCheckUTurn) {

		closestNeighbour = get_random_neighbour(*aboveTile);

		movementDirX = closestNeighbour.cols - aboveTile->cols;
		movementDirY = closestNeighbour.rows - aboveTile->rows;

		mustCheckUTurn = false;
	}
	
	Tile nextTile = *(find_next_tile(movementDirX, movementDirY));
	closestNeighbour = get_random_neighbour(nextTile);

	nextDirX = closestNeighbour.cols - nextTile.cols;
	nextDirY = closestNeighbour.rows - nextTile.rows;
}

bool Monster::leave_house() {

	sf::FloatRect monsterHouse = sf::FloatRect(11 * CELL_SIZE, 15 * CELL_SIZE, 6 * CELL_SIZE, 4 * CELL_SIZE);
	// std::cout << "Cute monster house: " << monsterHouse.left << " " << monsterHouse.top << "\n";

	if(!monsterHouse.contains(sf::Vector2f(centreX, centreY)) && !compare_float(centreX, 14.f * CELL_SIZE)) {
		// std::cout << "Monster house\n";
		return false;
	}

	sf::Vector2f centre = sf::Vector2f(centreX, centreY);
	sf::Vector2f target = sf::Vector2f(14.f * CELL_SIZE, 14.5f * CELL_SIZE);

	if(compare_point(centre, target, 0.005f)) {
		// std::cout << "Comparing vector2f -> Equal\n";
		movementDirX = nextDirX = -1;
		movementDirY = nextDirY = 0;
		
		aboveTile = grid->get_tile_at(14, 13);
		canEnterMonsterHouse = false;
		move_forward();

		return true;
	}

	float nextCentreY = centreY + movementSpeed * movementDirY;
	if(nextCentreY < target.y) {
		centreX = 14.f * CELL_SIZE;
		centreY = 14.5f * CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}


	move_forward();
	return true;
}

bool Monster::enter_house() {
	if(!isDead)
		return false;

	sf::Vector2f centre = sf::Vector2f(centreX, centreY);
	sf::Vector2f target = sf::Vector2f(14.f * CELL_SIZE, 17.5f * CELL_SIZE);

	if(compare_point(centre, target, 0.005f)) {
		// std::cout << "Comparing vector2f -> Equal\n";

		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;

		aboveTile = grid->get_tile_at(14, 13);

		isDead = false;
		
		move_forward();

		return true;
	}

	float nextCentreY = centreY + movementSpeed * movementDirY;
	if(nextCentreY > target.y) {
		centreX = 14.f * CELL_SIZE;
		centreY = 17.5f * CELL_SIZE;
		shape->setPosition(centreX, centreY);

		return true;
	}

	move_forward();
	return true;


}

// bool Monster::pass_door(sf::Vector2f target) {

// 	std::cout << "centre pinky: " << centreX << " " << centreY << "\n";	
// 	std::cout << "centre target: " << target.x << " " << target.y << "\n";	

// 	sf::Vector2f centre = sf::Vector2f(centreX, centreY);

// 	std::cout << "Comparing vector2f\n";

// 	if(isDead && movementDirY == -1)
// 		isDead = false;
	
// 	if(compare_point(centre, target, 0.005f)) {
// 		std::cout << "Comparing vector2f -> Equal\n";
// 		if(!isDead) {
// 			movementDirX = nextDirX = -1;
// 			movementDirY = nextDirY = 0;
// 		}
// 		else {
// 			movementDirX = nextDirX = 0;
// 			movementDirY = nextDirY = -1;
// 		}

// 		aboveTile = grid->get_tile_at(14, 13);
// 		move_forward();

// 		return true;
// 	}

// 	std::cout << "Comparing vector2f -> Not Equal\n";

// 	float nextCentreY = centreY + movementSpeed * movementDirY;
// 	if(!isDead && nextCentreY < target.y) {
// 		std::cout << "centring in pass door from right\n";
// 		centreX = 14.f * CELL_SIZE;
// 		centreY = 14.5f * CELL_SIZE;
// 		shape->setPosition(centreX, centreY);

// 		canEnterMonsterHouse = false;

// 		return true;
// 	}

// 	else if(isDead && nextCentreY > target.y) {
// 		std::cout << "centring in pass door from right\n";
// 		centreX = 14.f * CELL_SIZE;
// 		centreY = 17.5f * CELL_SIZE;
// 		shape->setPosition(centreX, centreY);

// 		return true;
// 	}

// 	std::cout << "Not equal so moving forward\n";

// 	move_forward();

// 	return true;
// }



bool Monster::handle_blinky_initial_move() {

	if(name.compare("Blinky"))
		return false;

	aboveTile = grid->get_tile_at(14, 13);

	movementDirX = nextDirX = -1;
	movementDirY = nextDirY = 0;

	move_forward();

	return true;
}

bool Monster::handle_inky_initial_move() {
	if(name.compare("Inky"))
		return false;

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float houseCentreX = 14.f * CELL_SIZE;

	if(compare_float(centreX, houseCentreX, 0.005f)) {
		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;

		return false;
	}
		
	if(nextCentreX > houseCentreX) {
		centreX = houseCentreX;
		shape->setPosition(centreX, centreY);

		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;
		
		return true;
	}
	
	centreX = nextCentreX;
	shape->setPosition(centreX, centreY);

	return true;
}

bool Monster::handle_clyde_initial_move() {
	if(name.compare("Clyde") || isDead)
		return false;

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float pinkySpawnCentreX = 14.f * CELL_SIZE;

	if(compare_float(centreX, pinkySpawnCentreX, 0.005f)) {
		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;

		return false;
	}

	if(nextCentreX < pinkySpawnCentreX) {
		centreX = pinkySpawnCentreX;
		shape->setPosition(centreX, centreY);

		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = -1;

		return true;
	}
	
	centreX = nextCentreX;
	shape->setPosition(centreX, centreY);

	return true;
}

bool Monster::handle_initial_move() {
	if(aboveTile) 
		return false;

	if(handle_blinky_initial_move())
		return true;

	if(handle_inky_initial_move())
		return true;
		
	if(handle_clyde_initial_move())
		return true;	

	if(leave_house())
		return true;

	return false;
}

bool Monster::is_near_centre() {

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float nextCentreY = centreY + movementSpeed * movementDirY;

	sf::Vector2f tileCentre = aboveTile->get_tile_centre();

	if(movementDirX == 1 && centreX < tileCentre.x && nextCentreX > tileCentre.x)
		return true;

	if(movementDirX == -1 && centreX > tileCentre.x && nextCentreX < tileCentre.x)
		return true;

	if(movementDirY == 1 && centreY < tileCentre.y && nextCentreY > tileCentre.y)
		return true;
	
	if(movementDirY == -1 && centreY > tileCentre.y && nextCentreY < tileCentre.y)
		return true;

	return false;
}

bool Monster::handle_centring() {

	if(is_near_centre()) {
		// std::cout << "Close to centre. Centring\n";
		centreX = aboveTile->get_tile_centre().x;
		centreY = aboveTile->get_tile_centre().y;
		shape->setPosition(centreX, centreY);

		movementDirX = nextDirX;
		movementDirY = nextDirY;

		return true;
	}

	return false;
}

void Monster::move_forward() {

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float nextCentreY = centreY + movementSpeed * movementDirY;

	if(aboveTile) {

		std::shared_ptr<Tile> nextTile = find_next_tile(movementDirX, movementDirY);

		if(nextTile->get_bounds().contains(sf::Vector2f(nextCentreX, nextCentreY)))
			aboveTile = nextTile;

	}

	centreX += movementSpeed * movementDirX;
	centreY += movementSpeed * movementDirY;
	
	shape->setPosition(centreX, centreY);
}

void Monster::make_uturn() {
	// Switch between left-right or up-down
	movementDirX *= -1;
	movementDirY *= -1;

	nextDirX = movementDirX;
	nextDirY = movementDirY;

	mustCheckUTurn = true;
}

bool Monster::reach_blinky_spawn() {
	if(!isDead)
		return false;

	sf::FloatRect monsterHouse = sf::FloatRect(11 * CELL_SIZE, 15 * CELL_SIZE, 6 * CELL_SIZE, 4 * CELL_SIZE);
	// std::cout << "Cute monster house: " << monsterHouse.left << " " << monsterHouse.top << "\n";
	// std::cout << "Centre: " << centreX << " " << centreY << "\n";
	if(monsterHouse.contains(sf::Vector2f(centreX, centreY))) {
		// std::cout << "Monster house\n";
		return false;
	}
		

	if(aboveTile->rows != 14 || (aboveTile->cols != 13 && aboveTile->cols != 14)) {
		// std::cout << "basic move in reach blinky spawn\n";
		basic_move();
		return true;
	}

	float nextCentreX = centreX + movementSpeed * movementDirX;
	float blinkySpawnCentreX = 14.f * CELL_SIZE;

	if(compare_float(centreX, blinkySpawnCentreX, 0.005f)) {
		// std::cout << "Go_to_blinky_spawn: preparing to go down";
		movementDirX = nextDirX = 0;
		movementDirY = nextDirY = 1;
		canEnterMonsterHouse = true;
		return false;
	}

	if((movementDirX == -1 && nextCentreX < blinkySpawnCentreX) || (movementDirX == 1 && nextCentreX > blinkySpawnCentreX)) {
		// std::cout << "Go_to_blinky_spawn: centering";
		centreX = blinkySpawnCentreX;
		shape->setPosition(centreX, 14.5f * CELL_SIZE);
		return true;
	}
	
	// std::cout << "move forward in reach blinky spawn\n";
	move_forward();

	return true;
}

// bool Monster::go_home() {
// 	if(!isDead || movementDirY != 1)
// 		return false;

// 	if(!compare_float(centreX, 14.f * CELL_SIZE, 0.005f))
// 		return false;

// 	if(centreY < 14.5f * CELL_SIZE || centreY > 17.5f * CELL_SIZE)
// 		return false;
	
// 	float nextCentreY = centreY + movementSpeed * movementDirY;
// 	float pinkySpawnCentreY = 17.5f * CELL_SIZE;

// 	if(compare_float(centreY, pinkySpawnCentreY, 0.005f)) {
// 		movementDirX = nextDirX = 0;
// 		movementDirY = nextDirY = -1;
// 		isDead = false;
// 		std::cout << "arrivé au milieu de pinky\n";
// 		return false;
// 	}

// 	if(nextCentreY > pinkySpawnCentreY) {
// 		centreY = pinkySpawnCentreY;
// 		shape->setPosition(centreX, centreY);
// 		std::cout << "positionnant au milieu de pinky\n";

// 		return true;
// 	}
	
// 	centreY = nextCentreY;
// 	shape->setPosition(centreX, centreY);

// 	return true;
// }

bool Monster::revive_monster() {

	// std::cout << "revive_monster\n";

	if(reach_blinky_spawn())
		return true;

	if(enter_house())
		return true;

	if(leave_house()) {
		return true;
	}
		
	return false;
}

void Monster::basic_move() {

	

	// if(handle_turn())
	// 	return;

	if(handle_centring()) {
		return;
	}


	move_forward();
}

// bool Monster::handle_turn() {

// 	sf::Vector2f centre = sf::Vector2f(centreX, centreY);

// 	if(isDead)
// 		std::cout << "Before handling new direction, Centre: " << centreX << " " << centreY << "\n";

// 	if(compare_point(centre, aboveTile->get_tile_centre(), 0.005f)) {
// 		//std::cout << "handle new direction\n";

// 		centreX += movementSpeed * movementDirX;
// 		centreY += movementSpeed * movementDirY;
// 		shape->setPosition(centreX, centreY);

// 		if(isDead)
// 		std::cout << "After, Centre: " << centreX << " " << centreY << "\n";

// 		return true;
// 	}

// 	//std::cout << "Not centred yet\n";

// 	return false;
// }

void Monster::move() {
	// std::cout << "Entering move():\n";
	// std::cout << "Centre: " << centreX << " " << centreY << "\n";

	// std::cout << "Initial move\n";
	if(handle_initial_move())
		return;

	// std::cout << "basic_move():\n\n";
	// std::cout << "Centre: " << centreX << " " << centreY << "\n";
	// std::cout << "AboveTile: " << aboveTile->rows << " " << aboveTile->cols << "\n";
	// std::cout << "Target: " << target.rows << " " << target.cols << "\n";
	// std::cout << "MovementDir: " << movementDirX << " " << movementDirY << "\n";
	// std::cout << "NextDir: " << nextDirX << " " << nextDirY << "\n";	

	// std::cout << "Handle tunnel\n";
	if(handle_tunnel())
		return;


	// std::cout << "Revive monster\n";
	if(revive_monster())
		return;

	// std::cout << "make basic move\n";
	basic_move();

}

bool Monster::is_character_collision() {
	if(!aboveTile || isDead)
		return false;

	if(pacman->aboveTile->rows == aboveTile->rows && pacman->aboveTile->cols == aboveTile->cols)
		return true;

	return false;
}

void Monster::handle_characters_collision() {
	if(!is_character_collision())
		return;

	if(mode == Mode::PANIC) {

		isDead = true;
		make_uturn();
		movementSpeed = 1.3f * REF_SPEED;
		shape->setFillColor(sf::Color(0, 0, 255, 127));
		canEnterMonsterHouse = true;

		if(centreX < 14 * CELL_SIZE)
			target = *(grid->get_tile_at(14, 13));
		else
			target = *(grid->get_tile_at(14, 14));


		std::cout << name << " DIED !\n";
		// kill_monster
		
		score->monster_eat();
	}
	else {
		std::cout << "You're dead !\n";
		// kill_pacman:
		// Losing screen: (while not reset), pacman not displayed, print "Too bad !", monsters and food remains
	}
}

/* --- PUBLIC FUNCTIONS --- */

// Constructors & Destructor

Monster::Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, std::shared_ptr<Score> score, 
				c_string& name) : Character(grid, name) {

	init_variables(pacman, score, name);
	init_time(name);
	init_target(name);
	init_shape(name);
	init_directions(name);
}

Monster::Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, std::shared_ptr<Score> score, 
				std::shared_ptr<Monster> blinky, c_string& name) : Character(grid, name) {

	init_variables(pacman, score, name, blinky);
	init_time(name);
	init_target(name);
	init_shape(name);
	init_directions(name);
}

void Monster::reset(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, c_string& name) {
	Character::init_variables(grid);
	init_monster(name);
	Monster::init_variables(pacman, score, name);
	init_time(name);
	init_target(name);
	init_shape(name);
	init_directions(name);

	dsecTimer = 0;
	secTimer = 0;
	panicCooldown = 0;
}

void Monster::update() {
	// To test for only one monster

	if(!pacman->aboveTile)
		return;

	if(name.compare("Pinky"))
		return;

	// Inky and Clyde cannot leave the monster house right away
	if(!can_leave_house()) {
		update_start_timer();
		// if(!name.compare("Clyde"))
		// 	std::cout << "Remaining time: " << startCountdown << "\n";
		return;
	} 

	update_panic();

	update_pattern_timer();

	update_pattern_mode();

	if(mode == Mode::PANIC)
		std::cout << name << " Panic: " << panicCooldown << "\n";
	if(patternMode == Mode::SCATTER)
		std::cout << name << " Scatter: " << patternModeCooldown << "\n";
	else if(patternMode == Mode::CHASE)
		std::cout << name << " Chase: " << patternModeCooldown << "\n";

	// Updates the target in function of the current mode
	update_target();

	handle_routing();

	make_random_decision();

	// Move
	move();

	handle_characters_collision();
}

void Monster::update_timer() {
	dsecTimer = 10 * ++secTimer / FPS;
	secTimer = secTimer % FPS;

	if(secTimer)
		return;

	if(panicCooldown > 0)
		--panicCooldown;
}

uint Monster::get_timer() {
	return secTimer;
}

void Monster::set_panic_cooldown() {
	panicCooldown += 7;
}