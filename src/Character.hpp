#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include "constants.hpp"
#include "Grid.hpp"
#include "Tile.hpp"

// Abstract class representing the player or a monster
class Character {

	protected:
		/* --- PROTECTED DATA MEMBERS --- */

		// Position
		std::unique_ptr<sf::Shape> shape;
		float centreX, centreY;

		// Movements
		float movementSpeed;
		int prevDirX, prevDirY, dirX, dirY;

		// Restrictions
		bool isMonsterHouseOpen;

		// Tile Map
		std::shared_ptr<Grid> grid;
		std::shared_ptr<Tile> aboveTile;	

		// Add sickness, immunity, ... fields

		/* --- PROTECTED FUNCTIONS --- */

		void init_variables(std::shared_ptr<Grid> grid);

		// Initialises the data members of the player
		void init_player();

		// Initialises the data members of a monster
		void init_monster(c_string& name);

		// Updates the data member 'aboveTile' in regards to the tile on which is the player/monster
		// Returns false if the character is using the underground tunnel
		std::shared_ptr<Tile> find_next_tile(c_int& dirX, c_int& dirY);

		// Returns true if the character is not moving
		bool is_motionless();

		// Reliable ???
		bool is_turning();

		// Returns true if the character is turning
		bool is_changing_direction();

		// Returns true if the character is U-Turning
		bool is_changing_orientation();

		// Returns true if the next tile is a wall or a restricted area (e.g., a door for PAC-MAN)
		bool predict_wall_collision();

		// Returns true if the character changed direction before reaching the middle of the tile
		bool is_right_angle_timing(c_float& centreX, c_float& centreY);

		// Handles right angle turns
		void handle_right_angle();

		// Returns true if the character is using the underground tunnel (hidden)
		bool is_under_tunnel();

		// Returns true if the character is on the tile next to the tunnel and
		// is moving in its direction
		bool reaching_tunnel();

		// Returns true if the character is on the tile next to the tunnel but 
		// is moving in the opposite direction
		bool moving_away_from_tunnel();

		// Handles the movements when the character is under the tunnel, reaching it,
		// or moving away from it. Returns false if the character is not at that position
		bool handle_tunnel();
		
	public:
		/* --- PUBLIC FUNCTIONS --- */

		// Constructors & Destructor
		Character(std::shared_ptr<Grid> grid);
		
		Character(std::shared_ptr<Grid> grid, c_string& name);

		virtual ~Character() = default;

		// Updates each data member
		virtual void update() = 0;

		// Draws the needed data members on the window
		void render(sf::RenderTarget* target);
};

#endif // !CHARACTER_HPP