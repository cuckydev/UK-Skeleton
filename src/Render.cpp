/*
 * UK Skeleton
 * File: Render.cpp
 * 
 * Purpose: Define the basic rendering interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "Render.h"
#include "GameDef.h"

#include <Backend/Render.h>

#include <algorithm>
#include <cstddef>

namespace Render
{
	//Error
	Error error;
	
	//Backend render surface
	uint32_t *screen_data;
	unsigned long screen_pitch;
	
	//Internal texture interface
	/*
	 * Render::Texture::DataFormat:
	 *   Converts the given texture data to the format needed by the screen data
	 * 
	 * Arguments:
	 *   texture_data - Texture data to use
	*/
	void Texture::DataFormat(const TextureData &texture_data)
	{
		//Allocate data
		delete[] data;
		width = texture_data.Width();
		height = texture_data.Height();
		data = new uint32_t[width * height];
		
		const uint32_t *from = (const uint32_t*)texture_data.Data();
		uint32_t *to = data;
		#ifdef _LIL_ENDIAN
			//Copy and swap data for little-endian (lodepng decodes as RRGGBBAA, we want AABBGGRR)
			for (unsigned int i = 0; i < width * height; i++, from++)
				*to++ = (*from >> 24) | ((*from >> 8) & 0xFF00) | ((*from << 8) & 0xFF0000) | (*from << 24);
		#else
			//Copy data
			std::copy_n(from, width * height, to);
		#endif
	}
	
	//Constructors and destructor
	/*
	 * Render::Texture::Texture:
	 *   Texture class constructor, reading from a file at the given path
	 * 
	 * Arguments:
	 *   name - File path
	*/
	Texture::Texture(std::string name)
	{
		//Load given texture
		TextureData texture_data(name);
		CheckInstanceError_Void(texture_data);
		DataFormat(texture_data);
	}
	
	/*
	 * Render::Texture::Texture:
	 *   Texture class constructor, reading from the given stream
	 * 
	 * Arguments:
	 *   name - Stream to read from
	*/
	Texture::Texture(std::istream &stream)
	{
		//Load given texture
		TextureData texture_data(stream);
		CheckInstanceError_Void(texture_data);
		DataFormat(texture_data);
	}
	
	/*
	 * Render::Texture::~Texture:
	 *   Texture class destructor
	*/
	Texture::~Texture()
	{
		//Delete loaded texture data
		delete[] data;
	}
	
	//Texture interface
	/*
	 * Render::Texture::Draw:
	 *   Blits the given rect of the texture onto the screen at the given coordinates
	 * 
	 * Arguments:
	 *   rect - Rect of the rectangle to blit, nullptr if entire texture is to be used
	 *   x - X to blit at
	 *   y - Y to blit at
	*/
	void Texture::Draw(const Rect *rect, int x, int y)
	{
		//Drop invalid draws
		if (x >= GAME_WIDTH || y >= GAME_HEIGHT)
			return;
		
		//Get clipped rect
		Rect crect;
		if (rect != nullptr)
			crect = *rect;
		else
			crect = {0, 0, (int)width, (int)height};
		
		//Drop invalid draws
		if (x <= -crect.w || y <= -crect.h)
			return;
		
		//Clip on X-axis
		if (x + crect.w > GAME_WIDTH)
			crect.w -= (x + crect.w) - GAME_WIDTH;
		
		if (x < 0)
		{
			crect.x -= x;
			crect.w += x;
			x = 0;
		}
		
		//Clip on Y-axis
		if (y + crect.h > GAME_HEIGHT)
			crect.h -= (y + crect.h) - GAME_HEIGHT;
		
		if (y < 0)
		{
			crect.y -= y;
			crect.h += y;
			y = 0;
		}
		
		//Blit texture
		const uint32_t *from = data + crect.y * width + crect.x;
		const size_t from_inc = width - crect.w;
		uint32_t *to = screen_data + y * screen_pitch + x;
		const size_t to_inc = screen_pitch - crect.w;
		
		for (unsigned int y = 0; y < crect.h; y++)
		{
			for (unsigned int x = 0; x < crect.w; x++, from++)
			{
				if (*from & 0xFF000000)
					*to++ = *from;
				else
					to++;
			}
			from += from_inc;
			to += to_inc;
		}
	}
	
	//Render interface
	/*
	 * Render::Start:
	 *   Starts a rendering frame
	 * 
	 * Returns:
	 *   Status code - (false on success, true on failure)
	*/
	bool Start()
	{
		//Start backend render frame
		CheckFunction(Backend::Render::Start(&screen_data, &screen_pitch), error)
		screen_pitch >>= 2;
		
		return false;
	}
	
	/*
	 * Render::End:
	 *   Ends a rendering frame
	 * 
	 * Returns:
	 *   Status code - (false on success, true on failure)
	*/
	bool End()
	{
		//End backend render frame
		CheckFunction(Backend::Render::End(), error)
		
		return false;
	}
	
	/*
	 * Render::DrawLine:
	 *   Draws a line from x0,y0 to x1,y1 with the given colour
	 *   (https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
	 * 
	 * Arguments:
	 *   x0,y0 - First point
	 *   x1,y1 - Second point
	 *   col - Colour to draw
	*/
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t col)
	{
		int dx =  std::abs(x1 - x0);
		int sx = x0 < x1 ? 1 : -1;
		
		int dy = -std::abs(y1 - y0);
		int sy = y0 < y1 ? 1 : -1;
		
		int err = dx + dy; //error value e_xy
		
		while (x0 != x1 || y0 != y1)
		{
			//Plot pixel
			if (!(x0 < 0 || x0 >= GAME_WIDTH || y0 < 0 || y0 >= GAME_HEIGHT))
				screen_data[y0 * screen_pitch + x0] = col;
			
			//Increment position
			int e2 = 2 * err;
			if (e2 >= dy) //e_xy + e_x > 0
			{
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx) //e_xy + e_y < 0
			{
				err += dx;
				y0 += sy;
			}
		}
	}
	
	/*
	 * Render::DrawRect:
	 *   Draws the given rect (or the full screen) with the given colour
	 * 
	 * Arguments:
	 *   rect - Rect area to draw
	 *   col - Colour to draw
	*/
	void DrawRect(const Rect *rect, uint32_t col)
	{
		//Get rect to draw
		Rect crect;
		if (rect != nullptr)
		{
			//Drop invalid draws
			if (rect->x >= GAME_WIDTH || rect->y >= GAME_HEIGHT || rect->x <= -rect->w || rect->y <= -rect->h)
				return;
			
			//Use the given rect
			crect = *rect;
			
			//Clip against top left
			if (crect.x < 0)
			{
				crect.w += crect.x;
				crect.x = 0;
			}
			if (crect.x > 0)
			{
				crect.h += crect.y;
				crect.y = 0;
			}
			
			//Clip against bottom right
			if (crect.x > GAME_WIDTH - crect.w)
				crect.w += (GAME_WIDTH - crect.w) - crect.x;
			if (crect.y > GAME_HEIGHT - crect.h)
				crect.h += (GAME_HEIGHT - crect.h) - crect.y;
		}
		else
		{
			//Use full-screen rect
			crect = {0, 0, GAME_WIDTH, GAME_HEIGHT};
		}
		
		//Blit rect
		uint32_t *to = screen_data + crect.y * screen_pitch + crect.x;
		const size_t to_inc = screen_pitch - crect.w;
		
		for (unsigned int y = 0; y < crect.h; y++)
		{
			for (unsigned int x = 0; x < crect.w; x++)
				*to++ = col;
			to += to_inc;
		}
	}
	
	/*
	 * Render::GetScreen:
	 *   Get the internal screen buffer
	 * 
	 * Returns:
	 *   screen - Native endianness 32-bit array of screen pixels
	 *   pitch - Pitch of the screen data (divided by 4)
	*/
	void GetScreen(uint32_t **screen, unsigned long *pitch)
	{
		if (screen != nullptr)
			*screen = screen_data;
		if (pitch != nullptr)
			*pitch = screen_pitch;
	}
}
