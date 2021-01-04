/*
 * UK Skeleton
 * File: Render.h
 * 
 * Purpose: Declare the basic rendering interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once

#include <cstdint>
#include <iostream>

#include <Util/Error.h>
#include <Util/TextureData.h>

namespace Render
{
	//Error
	extern Error error;
	
	//Render types
	struct Rect
	{
		int x, y, w, h;
	};
	
	//Texture class
	class Texture
	{
		private:
			//Error
			Error error;
			
			//Texture data
			uint32_t *data = nullptr;
			unsigned int width, height;
			
		private:
			//Internal interface
			void DataFormat(const TextureData &texture_data);
			
		public:
			//Constructors and destructor
			Texture(std::string name);
			Texture(std::istream &stream);
			~Texture();
			
			//Texture interface
			void Draw(const Rect *rect, int x, int y);
			
			const uint32_t *Data() const { return data; }
			const unsigned int &Width() const { return width; }
			const unsigned int &Height() const { return height; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
	
	//Render helper functions
	constexpr uint32_t ToColour(uint32_t x)
	{
		#ifdef _BIG_ENDIAN
			return (x >> 24) | ((x >> 8) & 0xFF00) | ((x << 8) & 0xFF0000) | (x << 24);
		#else
			return x;
		#endif
	}
	
	constexpr uint32_t ToColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		return (r << 24) | (g << 16) | (b << 8) | (a);
	}
	
	//Render interface
	bool Start();
	bool End();
	
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t col);
	void DrawRect(const Rect *rect, uint32_t col);
	
	void GetScreen(uint32_t **screen, unsigned long *pitch);
}
