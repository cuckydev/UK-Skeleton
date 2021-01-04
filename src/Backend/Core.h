/*
 * UK Skeleton
 * File: Backend/Core.h
 * 
 * Purpose: Declare backend core interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include <Util/Error.h>

namespace Backend
{
	namespace Core
	{
		//Core error
		extern Error error;
		
		//Core interface
		bool Initialize();
		void Quit();
	}
}
