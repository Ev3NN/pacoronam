#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Grid.hpp"

// Derived class representing the sole player Pac-Man

class Player : public Character {

	protected:

		/* --- PRIVATE DATA MEMBERS --- */

		bool takeCorner, isCornering;
		int digestCooldown;

		/* --- PRIVATE FUNCTIONS --- */

		void init_variables();

		// Initialises the shape
		void init_shape();


		bool is_digesting();

		bool handle_initial_move();

		bool handle_blocking_wall();

		bool handle_non_blocking_wall();

		bool handle_wall();

		void handle_treat();

		void handle_pill();

		void eat_food();

		bool handle_cornering();

		bool handle_turn();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Player(Grid* grid);
		~Player();

		void set_direction(c_int dirX, c_int dirY);

		void update_digestion_cooldown();

		void move();

		// Updates each data member
		void update();
};

#endif // !PLAYER_HPP