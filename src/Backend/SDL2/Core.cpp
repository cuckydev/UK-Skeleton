/*
 * UK Skeleton
 * File: Backend/SDL2/Core.cpp
 * 
 * Purpose: Define SDL2 core interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "../Core.h"

#include "SDL.h"

namespace Backend
{
	namespace Core
	{
		//Core error
		Error error;
		
		//Core interface
		/*
		 * Backend::Core::Initialize:
		 *   Initializes the core backend subsystem
		 * 
		 * Returns:
		 *   Status code (false on success, true on failure)
		*/
		bool Initialize()
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
				return error.Push(SDL_GetError());
			return false;
		}
		
		/*
		 * Backend::Core::Quit:
		 *   Deinitializes the core backend subsystem
		 * 
		 * Returns:
		 *   Status code (false on success, true on failure)
		*/
		void Quit()
		{
			SDL_Quit();
		}
	}
}
