#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "Grid.hpp"
#include "Tile.hpp"

// Abstract class representing the player or a monster
class Character {

	protected:
		/* --- PROTECTED DATA MEMBERS --- */

		// Visual appearance
		sf::Shape* shape;
		float centreX, centreY;

		// Movement related fields
		float movementSpeed;
		int dirX, dirY;

		Grid* grid;

		// The tile where the character is positioned
		Tile* aboveTile;

		// Add sickness, immunity, ... fields

		/* --- PROTECTED FUNCTIONS --- */

		// Updates the data member 'aboveTile' in regards to the tile on which is the player/monster
		// Returns false if the character is using the underground tunnel
		Tile* find_next_tile();

		// Initialises the data members of the player
		void init_player(Grid* grid, c_float movementSpeed, c_float centreX, c_float centreY);

		// Initialises the data members of a monster
		void init_monster(Grid* grid, c_string& name);

	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Character(Grid* grid);
		Character(Grid* grid, c_string& name);
		virtual ~Character();
		
		virtual void update() = 0;

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move(c_int dirX, c_int dirY);

		// Draws the needed data members on the window
		void render(sf::RenderTarget* target);
};

#endif // !CHARACTER_HPP