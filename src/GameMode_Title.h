/*
 * UK Skeleton
 * File: GameMode_Title.h
 * 
 * Purpose: Declare title game mode class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include "Game.h"

namespace Game
{
	//Title game mode class
	class GameMode_Title : public GameMode
	{
		private:
			
		public:
			//Constructor and destructor
			GameMode_Title();
			~GameMode_Title() override;
			
			//Game mode interface
			GameMode *Process(const GameState &game_state) override;
	};
}
