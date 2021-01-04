/*
 * UK Skeleton
 * File: Main.cpp
 * 
 * Purpose: Game entry point
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "Main.h"

#include "Game.h"
#include "DisplayError.h"

#include <Backend/Backend.h>
#include <Util/Error.h>

//Executable directory
std::string executable_dir;

//Entry point
/*
 * main:
 *   Game entry point
 * 
 * Arguments:
 *   argc - Number of arguments
 *   argv - Arguments (C strings)
 * 
 * Returns:
 *   Status code (0 on success, -1 on failure)
*/
int main(int argc, char *argv[])
{
	//Get executable directory
	if (argc > 0)
	{
		executable_dir = argv[0];
		executable_dir = executable_dir.substr(0, executable_dir.find_last_of("/\\") + 1);
	}
	else
	{
		executable_dir = "./";
	}
	
	//Run game
	Error error;
	Game::Game *game = new Game::Game();
	
	if (game == nullptr)
		error.Push("Failed to create game instance");
	else if (game->GetError() || game->Start())
		error.Push(game->GetError());
	delete game;
	
	if (error)
	{
		//Game errored, print and display error
		std::cout << error << '\n';
		DisplayError(error);
		return -1;
	}
	
	//No error occured
	return 0;
}
