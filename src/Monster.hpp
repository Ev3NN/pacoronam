#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Character.hpp"

class Monster : public Character {

	private:
		sf::RectangleShape shape;

        // Modes, target, ...

		// Private functions
		void init_shape(std::string name);

	public:
		Monster(std::string name);

		void update();

		void render(sf::RenderTarget* target);

};

#endif // !MONSTER_HPP