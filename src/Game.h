/*
 * UK Skeleton
 * File: Game.h
 * 
 * Purpose: Declare game class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once

#include <cstdint>

#include <Util/Error.h>

namespace Game
{
	//Game state structure
	struct GameState
	{
		//Delta time
		float dt;
		
		//Movement state
		float forward, strafe;
		float turn;
	};
	
	//Game mode base class
	class GameMode
	{
		protected:
			//Error
			Error error;
			
		public:
			//Virtual destructor
			virtual ~GameMode() {}
			
			//Game mode interface
			virtual GameMode *Process(const GameState &game_state) = 0;
			
			//Get error
			const Error &GetError() const { return error; }
	};

	//Game class
	class Game
	{
		private:
			//Error
			Error error;
			
			//Current game mode
			GameMode *game_mode = nullptr;
			
		public:
			//Constructor and destructor
			Game();
			~Game();
			
			//Game interface
			bool Start();
			
			//Get error
			const Error &GetError() const { return error; }
	};
}
