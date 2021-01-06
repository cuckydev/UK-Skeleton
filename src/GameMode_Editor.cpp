/*
 * UK Skeleton
 * File: GameMode_Editor.cpp
 * 
 * Purpose: Define editor game mode class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "GameMode_Editor.h"
#include "Main.h"

namespace Game
{
	//Constructor and destructor
	/*
	 * Game::GameMode_Editor::GameMode_Editor:
	 *   Editor game mode class constructor
	*/
	GameMode_Editor::GameMode_Editor()
	{
		//Create map
		map = new Map::Map(executable_dir + "Data/Map/Map01.txt");
		CheckInstanceCreate_Void(map, "Failed to create map instance")
	}
	
	/*
	 * Game::GameMode_Editor::~GameMode_Editor:
	 *   Editor game mode class destructor
	*/
	GameMode_Editor::~GameMode_Editor()
	{
		//Delete map
		delete map;
	}
	
	//Game mode interface
	/*
	 * Game::GameMode_Editor::Process:
	 *   Editor game mode class process
	 * 
	 * Arguments:
	 *   game_state - Game state
	 * 
	 * Returns:
	 *   Next game mode
	*/
	GameMode *GameMode_Editor::Process(const GameState &game_state)
	{
		//Run map process
		CheckInstanceMethodPointer_Null(map, Process(game_state))
		return this;
	}
}
