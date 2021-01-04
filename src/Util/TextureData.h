/*
 * UK Skeleton
 * File: Util/TextureData.cpp
 * 
 * Purpose: Declare texture data class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include <string>
#include <istream>
#include <cstdint>

#include <Util/Error.h>

//Texture types
class TextureData
{
	private:
		//Error
		Error error;
		
		//Texture information
		uint8_t *data = nullptr;
		unsigned int width, height;
		
	private:
		//Internal interface
		bool Read(std::istream &stream);
		
	public:
		//Constructors and destructor
		TextureData(std::string path);
		TextureData(std::istream &stream);
		~TextureData();
		
		//Texture data interface
		const uint8_t *Data() const { return data; }
		const unsigned int &Width() const { return width; }
		const unsigned int &Height() const { return height; }
		
		//Get error
		const Error &GetError() const { return error; }
};
