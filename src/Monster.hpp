#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "constants.hpp"
#include "Character.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include "Player.hpp"

enum class Mode {CHASE, SCATTER, PANIC};

// Derived class representing a specific monster
class Monster : public Character {

	protected:
		/* --- PROTECTED DATA MEMBERS --- */
		
		// Basic data members
		string name;
		bool isDead;

		// Behaviours
		Mode patternMode;
		Mode mode;

		// Local timers
		int patternModeCooldown;
		uint startCountdown;

		// Global timers
		static uint panicCooldown;
		static uint secTimer;
		static uint dsecTimer;
	
		// Map
		std::shared_ptr<Player> pacman;
		Tile target;
		bool hasTarget;

		// Direction
		bool mustCheckUTurn;

		// Only for Inky who needs his position to compute his target
		std::shared_ptr<Monster> blinky;

		std::shared_ptr<Score> score;

		/* --- 	PROTECTED FUNCTIONS --- */

		// Initialises the data members
		void init_variables(std::shared_ptr<Player> pacman, std::shared_ptr<Score> score, c_string name, 
							std::shared_ptr<Monster> monster = nullptr);

		// Initialises the shape (declared in base class)
		void init_shape(c_string& name);

		void init_target(c_string& name);

		void init_time(c_string& name);

		void init_directions(c_string& name);





		bool can_leave_house();





		void update_pattern_mode();

		void update_panic();




		void update_blinky_chase_target();

		void update_pinky_chase_target();

		void update_inky_chase_target();

		void update_clyde_chase_target();

		void update_chase_target();

		void update_scatter_target();	

		void update_target();



		void update_start_timer();

		void update_pattern_timer();






		void reset_monster_colour();




		std::vector<Tile> get_neighbours(Tile& refTile);

		Tile get_closest_neighbour(Tile& refTile);

		Tile get_random_neighbour(Tile& refTile);

		bool is_routing_available();

		void handle_routing();

		void make_random_decision();



		bool handle_blinky_initial_move();

		bool handle_inky_initial_move();

		bool handle_clyde_initial_move();

		bool pass_door(sf::Vector2f target);

		bool leave_house();

		bool enter_house();

		bool handle_initial_move();

		



		bool handle_turn();

		bool handle_centring();

		void move_forward();

		void make_uturn();

		bool is_near_centre();

		bool is_character_collision();

		void handle_characters_collision();

		bool reach_blinky_spawn();

		bool go_home();

		bool revive_monster();

		void basic_move();

		// Given a specific direction, moves the shape and keep 'aboveTile' updated
		void move();

	public:
		/* --- PUBLIC DATA MEMBERS --- */

		// Constructors & Destructor
		Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, std::shared_ptr<Score> score, c_string& name);
		Monster(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, std::shared_ptr<Score> score,
				std::shared_ptr<Monster> monster, c_string& name);
				
		// Resets PAC-MAN to his initial state
		void reset(std::shared_ptr<Grid> grid, std::shared_ptr<Player> pacman, c_string& name);
		
		// Updates each data member
		void update();


		static void update_timer();

		static uint get_timer();

		static void set_panic_cooldown();
};




#endif // !MONSTER_HPP