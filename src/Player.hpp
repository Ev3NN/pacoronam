#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Grid.hpp"

// Derived class representing the sole player Pac-Man

class Player : public Character {

	protected:

		/* --- PRIVATE DATA MEMBERS --- */

		// Will probably need some private data members

		/* --- PRIVATE FUNCTIONS --- */

		// Initialises the shape
		void init_shape();

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Player(Grid* grid);
		~Player();

		
		void update_digestion_cooldown();

		// Updates each data member
		void update();
};

#endif // !PLAYER_HPP