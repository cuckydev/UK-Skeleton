/*
 * UK Skeleton
 * File: Backend/SDL2/Render.cpp
 * 
 * Purpose: Define SDL2 render interface
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "../Render.h"

#include "SDL_render.h"

namespace Backend
{
	namespace Render
	{
		//Render error
		Error error;
		
		//SDL window and renderer
		static SDL_Window *window = nullptr;
		static SDL_Renderer *renderer = nullptr;
		static SDL_Texture *texture = nullptr;
		static unsigned int width, height;
		
		//Render interface
		/*
		 * Backend::Render::Initialize:
		 *   Initializes the render backend subsystem
		 * 
		 * Arguments:
		 *   title - Window title
		 *   width - Game width
		 *   height - Game height
		 *   mag - Window magnification
		 * 
		 * Returns:
		 *   Status code (false on success, true on failure)
		*/
		bool Initialize(std::string title, unsigned int _width, unsigned int _height, unsigned int mag, const uint8_t *icon_data, unsigned int icon_width, unsigned int icon_height)
		{
			//Use given dimensions
			width = _width;
			height = _height;
			
			//Create window
			if ((window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * mag, height * mag, 0)) == nullptr)
				return error.Push(SDL_GetError());
			
			//Use the given icon
			if (icon_data != nullptr)
			{
				SDL_Surface *icon_surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)icon_data, icon_width, icon_height, 32, icon_width * 4, SDL_PIXELFORMAT_RGBA32);
				if (icon_surface == nullptr)
					return error.Push(SDL_GetError());
				SDL_SetWindowIcon(window, icon_surface);
				SDL_FreeSurface(icon_surface);
			}
			
			//Create renderer
			if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC)) == nullptr)
				return error.Push(SDL_GetError());
			
			//Create texture
			if ((texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height)) == nullptr)
				return error.Push(SDL_GetError());
			
			return false;
		}
		
		/*
		 * Backend::Render::Quit:
		 *   Ends the render backend subsystem
		*/
		void Quit()
		{
			//Destroy window and renderer
			if (texture != nullptr)
				SDL_DestroyTexture(texture);
			if (renderer != nullptr)
				SDL_DestroyRenderer(renderer);
			if (window != nullptr)
				SDL_DestroyWindow(window);
		}
		
		/*
		 * Backend::Render::Start:
		 *   Starts a render frame
		 * 
		 * Returns:
		 *   data - Native endianness 32-bit array of screen pixels
		 *   pitch - Pitch of the screen data
		 *   
		 *   Status code (false on success, true on failure)
		*/
		bool Start(uint32_t **data, unsigned long *pitch)
		{
			//Lock texture
			int pintch;
			if (SDL_LockTexture(texture, nullptr, (void**)data, &pintch) < 0)
				return error.Push(SDL_GetError());
			if (pitch != nullptr)
				*pitch = pintch;
			
			return false;
		}
		
		/*
		 * Backend::Render::End:
		 *   Ends a render frame
		 * 
		 * Returns:
		 *   Status code (false on success, true on failure)
		*/
		bool End()
		{
			//Unlock and present texture
			SDL_UnlockTexture(texture);
			if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) < 0)
				return error.Push(SDL_GetError());
			SDL_RenderPresent(renderer);
			
			return false;
		}
	}
}
