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
		int prevDirX, prevDirY, dirX, dirY, nextTurnDirX, nextTurnDirY;

		bool takeCorner, takeRightAngle;
		bool isCornering;

		Grid* grid;

		// The tile where the character is positioned
		Tile* aboveTile;

		// Time
		int digestCooldown;

		// Add sickness, immunity, ... fields

		/* --- PROTECTED FUNCTIONS --- */

		

		// Initialises the data members of the player
		void init_player(Grid* grid, c_double movementSpeed, c_double centreX, c_double centreY);

		// Initialises the data members of a monster
		void init_monster(Grid* grid, c_string& name);

		// Updates the data member 'aboveTile' in regards to the tile on which is the player/monster
		// Returns false if the character is using the underground tunnel
		Tile* find_next_tile(c_int& dirX, c_int& dirY);

		bool is_digesting();

		bool is_motionless();

		bool handle_initial_move();

		bool handle_tunnel();

		bool handle_blocking_wall();

		bool handle_non_blocking_wall();

		bool predict_wall_collision();

		bool handle_wall();

		bool is_turning();

		bool is_changing_direction();

		bool is_changing_orientation();

		bool handle_treat();

		bool is_right_angle_timing();

		bool handle_right_angle();

		bool handle_cornering();

		bool handle_turn();
		
	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Character(Grid* grid);
		Character(Grid* grid, c_string& name);
		virtual ~Character();
		
		void set_direction(c_int dirX, c_int dirY);

		virtual void update() = 0;

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move();

		// Draws the needed data members on the window
		void render(sf::RenderTarget* target);
};

#endif // !CHARACTER_HPP