#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Character.hpp"

class Monster : public Character {

	private:
		string name;
		sf::RectangleShape shape;

        // Modes, target, ...

		// Private functions

		void init_variables(c_string name);
		void init_shape(c_string& name);

	public:
		Monster(c_string& name);
		
		void update();

		void render(sf::RenderTarget* target);

};

#endif // !MONSTER_HPP