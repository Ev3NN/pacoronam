#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Character.hpp"

class Player : public Character {

	protected:

		// Private functions
		void init_shape();

	public:
		Player();
		~Player();

		void update();
};

#endif // !PLAYER_HPP