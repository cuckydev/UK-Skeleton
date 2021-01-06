/*
 * UK Skeleton
 * File: Map.h
 * 
 * Purpose: Declare map class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#pragma once
#include "Game.h"
#include "GameDef.h"
#include "Render.h"

#include <list>
#include <string>
#include <iostream>

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
		size_t walls;
		const ReadWall *wall;
		const size_t *vertex;
	};
	
	struct ReadMap
	{
		size_t sectors;
		const Vertex *vertex;
		const ReadSector *sector;
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
			void DrawClipper(const Clipper &clipper);
			void Draw();
			
			void SetViewport(int w, int h);
			
			//Map data
			void UnloadData();
			bool LoadData(const ReadMap &read);
			bool ReadData(std::istream &stream);
			
			//Initializtion
			bool Initialize();
			
		public:
			//Constructors and destructor
			Map(const ReadMap &read);
			Map(std::string path);
			Map(std::istream &stream);
			~Map();
			
			//Map interface
			bool Process(const Game::GameState &game_state);
			
			//Get error
			const Error &GetError() const { return error; }
	};
}
