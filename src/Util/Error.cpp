/*
 * UK Skeleton
 * File: Util/Error.cpp
 * 
 * Purpose: Define error class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "Error.h"

//Error interface
/*
 * Error::ToString:
 *   Returns a string containing all the errors that have been recieved
 * 
 * Returns:
 *   The string containing all the errors that have been recieved
*/
std::string Error::ToString() const
{
	//Get string that contains all errors in the error list
	std::string errorString;
	
	for (size_t i = 0; i < error_list.size(); i++)
	{
		//Concatenate this error onto the full string, and concatenate a divider if not the last one
		errorString.append(error_list[i]);
		if (i < error_list.size() - 1)
			errorString.append(" | ");
	}
	
	return errorString;
}
