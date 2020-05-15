#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "constants.hpp"
#include "Character.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "Player.hpp"

enum class Mode {CHASE, SCATTER, PANIC};

// Derived class representing a specific monster
class Monster : public Character {

	protected:
		/* --- PROTECTED DATA MEMBERS --- */
		
		// Name of the monster
		string name;

		// Behaviours
		Mode patternMode;
		Mode mode;

		// Time
		int timer;
		int patternModeCooldown, panicCooldown;

		uint pillsCooldownSet;
		
		// TImer de sortie
		uint startCountdown;

		// Map
		std::shared_ptr<Player> player;
		Tile target;
		bool hasTarget;

		// Only for Inky who needs his position to compute his target
		std::shared_ptr<Monster> blinky;

		// Add a 'mustUTurn' field: when a monster enters PANIC Mode, the next move is a U-Turn

		/* --- 	PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(std::shared_ptr<Player> player, c_string name, 
							std::shared_ptr<Monster> monster = nullptr);

		// Initialises the shape (declared in base class)
		void init_shape(c_string& name);

		void init_target(c_string& name);

		void init_time(c_string& name);

		bool can_leave_house();

		void update_mode();

		void update_panic();

		void update_start_countdown();

		void update_time();

		void update_blinky_chase_target();

		void update_pinky_chase_target();

		void update_inky_chase_target();

		void update_clyde_chase_target();

		void update_chase_target();

		void update_scatter_target();	

		void update_target();

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move();

	public:
		/* --- PUBLIC DATA MEMBERS --- */

		// Constructors & Destructor
		Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, c_string& name);
		Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, 
				std::shared_ptr<Monster> monster, c_string& name);
				
		// Resets PAC-MAN to his initial state
		void reset(std::shared_ptr<Grid> grid, std::shared_ptr<Player> player, c_string& name);
		
		// Updates each data member
		void update();
};

#endif // !MONSTER_HPP