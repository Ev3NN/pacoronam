#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Character.hpp"

class Monster : public Character {

	protected:
		string name;

        // Modes, target, ...

		// Private functions

		void init_variables(c_string name);
		void init_shape(c_string& name);

	public:
		Monster(c_string& name);
		~Monster();
		
		void update();

};

#endif // !MONSTER_HPP