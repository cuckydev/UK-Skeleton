/*
 * UK Skeleton
 * File: Backend/Render.h
 * 
 * Purpose: Declare backend render interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include <Util/Error.h>
#include <string>
#include <cstdint>

namespace Backend
{
	namespace Render
	{
		//Render error
		extern Error error;
		
		//Render interface
		bool Initialize(std::string title, unsigned int width, unsigned int height, unsigned int mag, const uint8_t *icon_data, unsigned int icon_width, unsigned int icon_height);
		void Quit();
		
		bool Start(uint32_t **data, unsigned long *pitch);
		bool End();
	}
}
