#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Grid.hpp"
#include "Tile.hpp"

class Character {

	protected:
		sf::Shape* shape;
		float centreX, centreY;

		float movementSpeed;
		int dirX, dirY;
		Tile* aboveTile;

		// Sickness, immmune, ...

		void update_above_tile(Grid* grid, c_int& dirX, c_int& dirY);

		void init_variables(c_float movementSpeed, c_float centreX, c_float centreY);

	public:
		Character(c_float& movementSpeed, c_string& name);
		Character(c_float& movementSpeed, c_float& centreX, c_float& centreY);
		virtual ~Character();
		
		virtual void update() = 0;

		void move(Grid* grid, c_int dirX, c_int dirY);

		void render(sf::RenderTarget* target);
};


#endif // !CHARACTER_HPP