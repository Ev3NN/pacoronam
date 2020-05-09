#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>

#include "Tile.hpp"

class Character {

	protected:
		float movementSpeed;
		float dirX, dirY;
		Tile* aboveTile;

		// Sickness, immmune, ...

		void init_variables(c_float movementSpeed);

	public:
		Character(c_float& movementSpeed);
		virtual ~Character();
		
		virtual void update() = 0;

		virtual void render(sf::RenderTarget* target) = 0;
};


#endif // !CHARACTER_HPP