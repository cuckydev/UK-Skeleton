#pragma once
#include "Game.h"
#include "GameDef.h"
#include "Render.h"

#include <list>

#include <Util/Error.h>

namespace Map
{
	//Map constants
	constexpr size_t nullind = (size_t)-1;
	
	//Map types
	struct Vertex
	{
		float x, y;
	};
	
	struct Sector;
	
	struct Wall
	{
		//Flags
		
		//Texture
		const Sector *sector;
		uint32_t upper_colour, middle_colour, lower_colour;
	};
	
	struct Sector
	{
		//Positioning
		float floor_height, ceiling_height;
		
		//Texture
		uint32_t floor_colour, ceiling_colour;
		
		//Walls
		Wall *wall = nullptr;
		Vertex *vertex = nullptr;
		size_t walls;
	};
	
	//Reading map types
	struct ReadWall
	{
		//Flags
		
		//Texture
		size_t sector;
		uint32_t upper_colour, middle_colour, lower_colour;
	};
	
	struct ReadSector
	{
		//Positioning
		float floor_height, ceiling_height;
		
		//Texture
		uint32_t floor_colour, ceiling_colour;
		
		//Walls
		const ReadWall *wall;
		const size_t *vertex;
		size_t walls;
	};
	
	struct ReadMap
	{
		const Vertex *vertex;
		const ReadSector **sector;
		size_t sectors;
	};
	
	//Render types
	struct View
	{
		//Viewport
		int w, h;
		float hfov, vfov;
		
		//Positioning
		float x, y, z;
		float angle;
		float sin, cos;
	};
	
	struct Clipper
	{
		int left, right;
		int top[GAME_WIDTH];
		int bottom[GAME_WIDTH];
	};
	
	struct SectorDraw
	{
		const Sector *sector;
		Clipper clipper;
	};
	
	//Map class
	class Map
	{
		private:
			//Error
			Error error;
			
			//Render state
			uint32_t *screen_data;
			size_t screen_pitch;
			
			View view;
			
			std::list<SectorDraw> draw_sector;
			
			//Map data
			Sector *sector = nullptr;
			size_t sectors;
			
			//Test
			Render::Texture *texture = nullptr;
			
		private:
			//Rendering
			void ClipNear(Vertex &t, const Vertex &f);
			void DrawWall(const SectorDraw &sector_draw, Vertex v0, Vertex v1, Wall wall);
			void DrawSector(const SectorDraw sector_draw);
			void Draw();
			
			void SetViewport(int w, int h);
			
			//Map data
			void UnloadData();
			bool ReadData(const ReadMap &read);
			
		public:
			//Constructor and destructor
			Map();
			~Map();
			
			//Map interface
			bool Process(const Game::GameState &game_state);
			
			//Get error
			const Error &GetError() const { return error; }
	};
}
