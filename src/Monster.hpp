#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <string>

#include "constants.hpp"
#include "Character.hpp"
#include "Grid.hpp"

// Derived class representing a specific monster

class Monster : public Character {

	protected:
		/* --- PROTECTED DATA MEMBERS --- */
		
		// Name of the monster
		string name;

        // Add modes, target, ...

		/* --- PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(c_string name);

		// Initialises the shape (declared in base class)
		void init_shape(c_string& name);

	public:
		/* --- PUBLIC DATA MEMBERS --- */

		// Constructors & Destructor
		Monster(Grid* grid, c_string& name);
		~Monster();

		void move();

		void reset(Grid* grid, c_string& name);
		
		// Not implemented yet !
		// Updates each data member
		void update();

};

#endif // !MONSTER_HPP