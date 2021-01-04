/*
 * UK Skeleton
 * File: GameMode_Editor.h
 * 
 * Purpose: Declare editor game mode class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include "Game.h"
#include "Map.h"

namespace Game
{
	//Editor game mode class
	class GameMode_Editor : public GameMode
	{
		private:
			//Map
			Map::Map *map = nullptr;
			
		public:
			//Constructor and destructor
			GameMode_Editor();
			~GameMode_Editor() override;
			
			//Game mode interface
			GameMode *Process(const GameState &game_state) override;
	};
}
