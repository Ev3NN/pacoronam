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
		Mode mode;

		// Time
		int timer;
		int modeCooldown, panicCooldown;

		uint pillsCooldownSet;
		// TImer de sortie

		// Map
		Tile* target;
		Player* player;

		/* --- 	PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(Player* player, c_string name);

		// Initialises the shape (declared in base class)
		void init_shape(c_string& name);

		void update_mode();

		void update_panic();

		void update_time();

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