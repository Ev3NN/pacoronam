#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include "Character.hpp"
#include "Grid.hpp"
#include "Score.hpp"

// Derived class representing the sole player Pac-Man

class Player : public Character {

	protected:

		/* --- PROTECTED DATA MEMBERS --- */
		
		bool takeCorner, isCornering;
		int digestCooldown;

		bool isExtendingPill;

		// Score
		std::shared_ptr<Score> score;
		
		// Might add a boolean set to true when extending pill power
		// to compute the score properly

		/* --- PROTECTED FUNCTIONS --- */

		// Initialises data members specific to Player
		void init_variables();

		// Initialises the shape
		void init_shape();

		// Initialises the score
		void init_score(std::shared_ptr<Score> score);

		// Returns true if PAC-MAN is still digesting
		bool is_digesting();

		// Reduce the timer
		void update_digestion_cooldown();
		
		// Handles the special case of the initial move.
		// Returns true if it is not the first move
		bool handle_initial_move();

		// Handles cases where PAC-MAN is running into a wall
		// Returns true if PAC-MAN prepares to turn
		bool handle_blocking_wall();

		// Handles cases where PAC-MAN prepares a cornering
		// Returns true if PAC-MAN is not preparing to turn
		bool handle_non_blocking_wall();

		// Handles cases where there is a wall in PAC-MAN's direction
		bool handle_wall();

		// Handles corner turns
		// Returns false if PAC-MAN cannot make a corner-turn
		bool handle_cornering();

		// Handles turning decisions
		// Returns false if PAC-MAN does not turn
		bool handle_turn();

		// Handles cases where PAC-MAN eats a treat 
		void handle_treat();

		// Handles cases where PAC-MAN eats a pill and creates the new state
		void handle_pill();

		// Handles cases where PAC-MAN eats food (treat or pill)
		void eat_food();

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Player(std::shared_ptr<Grid> grid, std::shared_ptr<Score> score);

		void set_extending(c_bool isExtendingPill);

		// Sets the directions given the horiontal and vertical directions
		void set_direction(c_int nextDirX, c_int nextDirY);

		Tile* get_above_tile();

		// Resets PAC-MAN to his initial state
		void reset(std::shared_ptr<Grid>);

		// Updates each data member
		void update();

	friend class Monster;
};

#endif // !PLAYER_HPP