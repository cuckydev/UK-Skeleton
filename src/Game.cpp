/*
 * UK Skeleton
 * File: Game.cpp
 * 
 * Purpose: Define game class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/


#include "Game.h"
#include "GameMode_Editor.h"
#include "GameDef.h"
#include "Render.h"
#include "Main.h"

#include <Util/TextureData.h>

#include <Backend/Backend.h>

#include <ctime>

namespace Game
{
	//Constructor and destructor
	/*
	 * Game::Game::Game:
	 *   Game class constructor
	*/
	Game::Game()
	{
		//Load game icon
		TextureData icon_texdata(executable_dir + "Data/Icon.png");
		
		//Initialize backend
		CheckFunction_Void(Backend::Core::Initialize(), Backend::Core::error)
		CheckFunction_Void(Backend::Render::Initialize(
			"UK Skeleton",         /*title*/
			GAME_WIDTH,            /*width*/
			GAME_HEIGHT,           /*height*/
			GAME_SCALE,            /*mag*/
			icon_texdata.Data(),   /*icon_data*/
			icon_texdata.Width(),  /*icon_width*/
			icon_texdata.Height()  /*icon_height*/
		), Backend::Render::error)
		CheckFunction_Void(Backend::Input::Initialize(), Backend::Input::error)
		
		//Start in title screen mode
		game_mode = new GameMode_Editor();
		CheckInstanceCreate_Void(game_mode, "Failed to create initial game mode instance")
	}
	
	/*
	 * Game::Game::~Game:
	 *   Game class destructor
	*/
	Game::~Game()
	{
		//Delete game mode
		delete game_mode;
		
		//Quit backend
		Backend::Input::Quit();
		Backend::Render::Quit();
		Backend::Core::Quit();
	}

	//Game interface
	/*
	 * Game::Game::Start:
	 *   Starts the game loop
	 * 
	 * Returns:
	 *   Status code (false on success, true on failure)
	*/
	bool Game::Start()
	{
		GameState game_state;
		
		//Get initial tick
		struct timespec tick, now_tick;
		clock_gettime(CLOCK_BOOTTIME, &tick);
		
		//Enter game loop
		while (game_mode != nullptr)
		{
			//Handle events and quit on exit event
			if (Backend::Input::HandleEvents())
				break;
			
			//Get delta time
			clock_gettime(CLOCK_BOOTTIME, &now_tick);
			game_state.dt = (now_tick.tv_sec - tick.tv_sec) + 1e-9 * (now_tick.tv_nsec - tick.tv_nsec);
			clock_gettime(CLOCK_BOOTTIME, &tick);
			
			//Get input state
			game_state.forward = (float)IsInputDown(Backend::Input::InputCode_W) - (float)IsInputDown(Backend::Input::InputCode_S);
			game_state.strafe = 0.0f;
			game_state.turn = (float)IsInputDown(Backend::Input::InputCode_D) - (float)IsInputDown(Backend::Input::InputCode_A);
			
			//Start frame
			Render::Start();
			
			//Run current game mode process
			GameMode *next_mode = game_mode->Process(game_state);
			if (game_mode->GetError())
			{
				//Game errored
				error.Push(game_mode->GetError());
				delete game_mode;
				delete next_mode;
				game_mode = nullptr;
				return true;
			}
			else if (next_mode != game_mode)
			{
				//Switch to new game mode
				delete game_mode;
				game_mode = next_mode;
			}
			
			//End frame
			Render::End();
		}
		
		//Exit with no error
		return false;
	}
}
