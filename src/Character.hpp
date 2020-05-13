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
		int prevDirX, prevDirY, dirX, dirY;

		bool isMonsterHouseOpen;

		Grid* grid;

		// The tile where the character is positioned
		Tile* aboveTile;

		// Time

		// Add sickness, immunity, ... fields

		/* --- PROTECTED FUNCTIONS --- */

		

		// Initialises the data members of the player
		void init_player(Grid* grid, c_double movementSpeed, c_double centreX, c_double centreY);

		// Initialises the data members of a monster
		void init_monster(Grid* grid, c_string& name);

		// Updates the data member 'aboveTile' in regards to the tile on which is the player/monster
		// Returns false if the character is using the underground tunnel
		Tile* find_next_tile(c_int& dirX, c_int& dirY);

		

		bool is_motionless();

		

		bool is_under_tunnel();

		bool reaching_tunnel();

		bool moving_away_from_tunnel();

		bool handle_tunnel();

		

		bool predict_wall_collision();

		

		bool is_turning();

		bool is_changing_direction();

		bool is_changing_orientation();

		bool is_right_angle_timing(c_float& centreX, c_float& centreY);

		void handle_right_angle();

		
		
	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Character(Grid* grid);
		Character(Grid* grid, c_string& name);
		virtual ~Character();
		
		

		virtual void update() = 0;

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		virtual void move() = 0;

		// Draws the needed data members on the window
		void render(sf::RenderTarget* target);
};

#endif // !CHARACTER_HPP