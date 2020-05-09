#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Character.hpp"

class Player : public Character {

	private:
		sf::CircleShape shape;

		// Private functions
		void init_shape();

	public:
		Player();

		void update();

		void move(c_float& dirX, c_float& dirY);

		void render(sf::RenderTarget* target);

};

#endif // !PLAYER_HPP