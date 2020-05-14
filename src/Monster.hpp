#ifndef MONSTER_HPP
#define MONSTER_HPP

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
		Tile* target;
		Player* player;

		// Add a 'mustUTurn' field: when a monster enters PANIC Mode, the next move is a U-Turn

		/* --- 	PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(Player* player, c_string name);

		// Initialises the shape (declared in base class)
		void init_shape(c_string& name);

		bool can_start_hunting();

		void update_mode();

		void update_panic();

		void update_time();

		void update_chase_target();

		void update_scatter_target();	

		void update_target();

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move();

	public:
		/* --- PUBLIC DATA MEMBERS --- */

		// Constructors & Destructor
		Monster(Grid* grid, Player* player, c_string& name);
		~Monster();

		// Resets PAC-MAN to his initial state
		void reset(Grid* grid, Player* player, c_string& name);
		
		// Updates each data member
		void update();
};

#endif // !MONSTER_HPP