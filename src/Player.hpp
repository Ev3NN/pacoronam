#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Character.hpp"

class Player : public Character {

	private:
		sf::CircleShape shape;

		// Private functions
		void init_shape();

	public:
		Player();

		void update();

		void render(sf::RenderTarget* target);

};

#endif // !PLAYER_HPP