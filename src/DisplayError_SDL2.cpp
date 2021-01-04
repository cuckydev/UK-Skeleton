/*
 * UK Skeleton
 * File: DisplayError_SDL2.cpp
 * 
 * Purpose: Provides the error message box function for the SDL2 backend
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "DisplayError.h"
#include "SDL.h"

/*
 * DisplayError:
 *   Displays an error message in the form of an SDL2 message box
*/
void DisplayError(std::string error)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.c_str(), nullptr);
}
