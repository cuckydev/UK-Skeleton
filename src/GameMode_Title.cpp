/*
 * UK Skeleton
 * File: GameMode_Title.cpp
 * 
 * Purpose: Define title game mode class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "GameMode_Title.h"

namespace Game
{
	//Constructor and destructor
	/*
	 * Game::GameMode_Title::GameMode_Title:
	 *   Title game mode class constructor
	*/
	GameMode_Title::GameMode_Title()
	{
		
	}
	
	/*
	 * Game::GameMode_Title::~GameMode_Title:
	 *   Title game mode class destructor
	*/
	GameMode_Title::~GameMode_Title()
	{
		
	}
	
	//Game mode interface
	/*
	 * Game::GameMode_Title::Process:
	 *   Title game mode class process
	 * 
	 * Arguments:
	 *   game_state - Game state
	 * 
	 * Returns:
	 *   Next game mode
	*/
	GameMode *GameMode_Title::Process(const GameState &game_state)
	{
		return this;
	}
}
