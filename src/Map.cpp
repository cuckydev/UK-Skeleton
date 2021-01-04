#include "Map.h"
#include "Render.h"
#include "GameDef.h"
#include "Main.h"

#include <cmath>
constexpr double pi = 3.14159265358979323846;

namespace Map
{
	//vertices
	static const Vertex vert[] = {
		{ 200.0f, -100.0f},
		{-200.0f, -100.0f},
		{-100.0f,  100.0f},
		{ 100.0f,  100.0f},
		{ 400.0f,  500.0f},
		{ 100.0f,  500.0f},
		{-100.0f,  500.0f},
		{-400.0f,  500.0f},
		{-400.0f,  800.0f},
		{ -20.0f,  800.0f},
		{  20.0f,  800.0f},
		{ 400.0f,  800.0f},
		{-400.0f, 1100.0f},
		{ 400.0f, 1100.0f},
	};
	
	//sector 0
	static const ReadWall wall_0[] = {
		{      1, 0x581B90FF, 0x808080FF, 0x180800FF},
		{nullind, 0x581B90FF, 0x909090FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xA0A0A0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xB0B0B0FF, 0x180800FF},
	};
	
	static const size_t vertex_0[] = {
		2, 3, 0, 1
	};
	
	static const ReadSector sector_0 = {
		-41.0f,
		200.0f,
		0x201000FF,
		0x581B90FF,
		wall_0,
		vertex_0,
		4,
	};
	
	//sector 1
	static const ReadWall wall_1[] = {
		{      0, 0x581B90FF, 0x808080FF, 0x180800FF},
		{nullind, 0x581B90FF, 0x909090FF, 0x180800FF},
		{      3, 0x581B90FF, 0xA0A0A0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xB0B0B0FF, 0x180800FF},
		{      2, 0x581B90FF, 0xC0C0C0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xD0D0D0FF, 0x180800FF},
	};
	
	static const size_t vertex_1[] = {
		3, 2, 7, 6, 5, 4
	};
	
	static const ReadSector sector_1 = {
		-30.0f,
		200.0f,
		0x201000FF,
		0x581B90FF,
		wall_1,
		vertex_1,
		6,
	};
	
	//sector 2
	static const ReadWall wall_2[] = {
		{      1, 0x581B90FF, 0x808080FF, 0x180800FF},
		{nullind, 0x581B90FF, 0x909090FF, 0x180800FF},
		{      4, 0x581B90FF, 0xA0A0A0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xB0B0B0FF, 0x180800FF},
	};
	
	static const size_t vertex_2[] = {
		4, 5, 10, 11, 
	};
	
	static const ReadSector sector_2 = {
		-20.0f,
		200.0f,
		0x201000FF,
		0x581B90FF,
		wall_2,
		vertex_2,
		4,
	};
	
	//sector 3
	static const ReadWall wall_3[] = {
		{      1, 0x581B90FF, 0x808080FF, 0x180800FF},
		{nullind, 0x581B90FF, 0x909090FF, 0x180800FF},
		{      4, 0x581B90FF, 0xA0A0A0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xB0B0B0FF, 0x180800FF},
	};
	
	static const size_t vertex_3[] = {
		6, 7, 8, 9, 
	};
	
	static const ReadSector sector_3 = {
		-20.0f,
		200.0f,
		0x201000FF,
		0x581B90FF,
		wall_3,
		vertex_3,
		4,
	};
	
	//sector 4
	static const ReadWall wall_4[] = {
		{      2, 0x581B90FF, 0x808080FF, 0x180800FF},
		{nullind, 0x581B90FF, 0x909090FF, 0x180800FF},
		{      3, 0x581B90FF, 0xA0A0A0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xB0B0B0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xC0C0C0FF, 0x180800FF},
		{nullind, 0x581B90FF, 0xD0D0D0FF, 0x180800FF},
	};
	
	static const size_t vertex_4[] = {
		11, 10, 9, 8, 12, 13
	};
	
	static const ReadSector sector_4 = {
		-8.0f,
		200.0f,
		0x201000FF,
		0x581B90FF,
		wall_4,
		vertex_4,
		6,
	};
	
	//Map data
	static const ReadSector *sector_data[] = {
		&sector_0,
		&sector_1,
		&sector_2,
		&sector_3,
		&sector_4,
	};
	
	static const ReadMap map_data = {
		vert,
		sector_data,
		5,
	};
	
	
	//Constants
	static constexpr float near = 0.01f;
	
	//Map rendering
	void Map::ClipNear(Vertex &t, const Vertex &f)
	{
		//TODO: this is probably wrong LOL
		float per = (f.y - near) / (f.y - t.y);
		t.y = near;
		t.x = f.x + (t.x - f.x) * per;
	}
	
	void Map::DrawWall(const SectorDraw &sector_draw, Vertex v0, Vertex v1, Wall wall)
	{
		//Translate coordinates to view position
		float tx0 = v0.x - view.x;
		float ty0 = v0.y - view.y;
		float tx1 = v1.x - view.x;
		float ty1 = v1.y - view.y;
		
		//Rotate around view
		v0.x = tx0 * view.sin - ty0 * view.cos;
		v0.y = tx0 * view.cos + ty0 * view.sin;
		v1.x = tx1 * view.sin - ty1 * view.cos;
		v1.y = tx1 * view.cos + ty1 * view.sin;
		
		//Cull if fully behind
		if (v0.y <= near && v1.y <= near)
			return;
		
		//Clip line against near plane
		if (v0.y <= near)
			ClipNear(v0, v1);
		if (v1.y <= near)
			ClipNear(v1, v0);
		
		//Perspective projection
		float xscale0 = view.hfov / v1.y;
		float yscale0 = view.vfov / v1.y;
		int x0 = (int)((view.w / 2.0f) - (v1.x * xscale0));
		
		float xscale1 = view.hfov / v0.y;
		float yscale1 = view.vfov / v0.y;
		int x1 = (int)((view.w / 2.0f) - (v0.x * xscale1));
		
		//Immediately drop if wall is off-screen
		if (x1 <= sector_draw.clipper.left || x0 >= sector_draw.clipper.right)
			return;
		
		//Backface culling
		if (x1 <= x0)
			return;
		
		//Calculate projected floor and ceiling heights
		float ceiling_height = sector_draw.sector->ceiling_height - view.z;
		float floor_height =   sector_draw.sector->floor_height   - view.z;
		float y0a = (view.h / 2.0f) - (ceiling_height * yscale0), y0b = (view.h / 2.0f) - (floor_height * yscale0);
		float y1a = (view.h / 2.0f) - (ceiling_height * yscale1), y1b = (view.h / 2.0f) - (floor_height * yscale1);
		
		//Prepare heights for interpolation
		float w = 1.0f / (x1 - x0);
		
		double ya = y0a;
		double yai = (y1a - y0a) * w;
		
		double yb = y0b;
		double ybi = (y1b - y0b) * w;
		
		//Check if we're a wall or portal
		if (wall.sector != nullptr)
		{
			//Portal
			//Calculate projected floor and ceiling heights
			float portal_ceiling_height = wall.sector->ceiling_height - view.z;
			float portal_floor_height =   wall.sector->floor_height   - view.z;
			
			float y0c, y1c;
			double yc, yci;
			if (portal_ceiling_height < ceiling_height)
			{
				y0c = (view.h / 2.0f) - (portal_ceiling_height * yscale0);
				y1c = (view.h / 2.0f) - (portal_ceiling_height * yscale1);
				yc = y0c;
				yci = (y1c - y0c) * w;
			}
			else
			{
				y0c = y0a;
				yc = ya;
				yci = yai;
			}
			
			float y0d, y1d;
			double yd, ydi;
			if (portal_floor_height > floor_height)
			{
				y0d = (view.h / 2.0f) - (portal_floor_height * yscale0);
				y1d = (view.h / 2.0f) - (portal_floor_height * yscale1);
				yd = y0d;
				ydi = (y1d - y0d) * w;
			}
			else
			{
				y0d = y0b;
				yd = yb;
				ydi = ybi;
			}
			
			//Clip against left and right
			if (x0 < sector_draw.clipper.left)
			{
				ya -= yai * (x0 - sector_draw.clipper.left);
				yb -= ybi * (x0 - sector_draw.clipper.left);
				yc -= yci * (x0 - sector_draw.clipper.left);
				yd -= ydi * (x0 - sector_draw.clipper.left);
				x0 = sector_draw.clipper.left;
			}
			if (x1 > sector_draw.clipper.right)
				x1 = sector_draw.clipper.right;
			
			//Draw sector on the other side of the portal
			Clipper clipper;
			clipper.left = x0;
			clipper.right = x1;
			
			//Draw wall
			for (int x = x0; x < x1; x++)
			{
				int t = sector_draw.clipper.top[x];
				int b = sector_draw.clipper.bottom[x];
				
				int y = t;
				uint32_t *cp = screen_data + y * screen_pitch + x;
				
				int ceiling_to = std::min((int)ya, b);
				for (; y < ceiling_to; y++, cp += screen_pitch)
					*cp = sector_draw.sector->ceiling_colour;
				
				int upper_to = std::min((int)yc, b);
				for (; y < upper_to; y++, cp += screen_pitch)
					*cp = wall.upper_colour;
				
				int middle_to = std::min((int)yd, b);
				
				int lower_to = std::min((int)yb, b);
				for (; y < lower_to; y++, cp += screen_pitch)
					*cp = wall.lower_colour;
				
				int floor_to = b;
				for (; y < floor_to; y++, cp += screen_pitch)
					*cp = sector_draw.sector->floor_colour;
				
				clipper.top[x] = std::max(upper_to, t);
				clipper.bottom[x] = std::min(middle_to, b);
				
				ya += yai;
				yb += ybi;
				yc += yci;
				yd += ydi;
			}
			
			//Draw sector on the other end of the portal
			draw_sector.push_back({wall.sector, clipper});
		}
		else
		{
			//Wall
			//Clip against left and right
			if (x0 < sector_draw.clipper.left)
			{
				ya -= yai * (x0 - sector_draw.clipper.left);
				yb -= ybi * (x0 - sector_draw.clipper.left);
				x0 = sector_draw.clipper.left;
			}
			if (x1 > sector_draw.clipper.right)
				x1 = sector_draw.clipper.right;
			
			//Draw wall
			for (int x = x0; x < x1; x++)
			{
				int t = sector_draw.clipper.top[x];
				int b = sector_draw.clipper.bottom[x];
				
				int y = t;
				uint32_t *cp = screen_data + y * screen_pitch + x;
				
				int ceiling_to = std::min((int)ya, b);
				for (; y < ceiling_to; y++, cp += screen_pitch)
					*cp = sector_draw.sector->ceiling_colour;
				
				int wall_to = std::min((int)yb, b);
				for (; y < wall_to; y++, cp += screen_pitch)
					*cp = wall.middle_colour;
				
				int floor_to = b;
				for (; y < floor_to; y++, cp += screen_pitch)
					*cp = sector_draw.sector->floor_colour;
				
				ya += yai;
				yb += ybi;
			}
		}
	}
	
	void Map::DrawSector(const SectorDraw sector_draw)
	{
		//Draw walls and portals
		for (size_t i = 0; i < sector_draw.sector->walls; i++)
			DrawWall(sector_draw, sector_draw.sector->vertex[i], sector_draw.sector->vertex[i + 1], sector_draw.sector->wall[i]);
	}
	
	void Map::Draw()
	{
		size_t view_sector;
		if (view.y >= 800.0f)
			view_sector = 4;
		else if (view.y >= 500.0f)
			view_sector = (view.x < 0.0f) ? 3 : 2;
		else if (view.y >= 100.0f)
			view_sector = 1;
		else
			view_sector = 0;
		
		//Initial draw sector
		Clipper clipper;
		clipper.left = 0;
		clipper.right = view.w;
		
		int *topp = clipper.top;
		int *bottomp = clipper.bottom;
		for (int i = 0; i < view.w; i++)
		{
			*topp++ = 0;
			*bottomp++ = view.h;
		}
		
		draw_sector.clear();
		draw_sector.push_back({&sector[view_sector], clipper});
		
		//Draw sectors
		for (auto &i : draw_sector)
			DrawSector(i);
	}
	
	void Map::SetViewport(int w, int h)
	{
		//Set projection
		view.w = std::max(std::min(w, GAME_WIDTH), 0);
		view.h = std::max(std::min(h, GAME_HEIGHT), 0);
		view.hfov = 0.7f * view.h;
		view.vfov = 0.7f * view.h;
	}
	
	//Map data
	void Map::UnloadData()
	{
		if (sector != nullptr)
		{
			for (size_t i = 0; i < sectors; i++)
			{
				delete[] sector[i].wall;
				delete[] sector[i].vertex;
			}
			delete[] sector;
			sector = nullptr;
		}
	}
	
	bool Map::ReadData(const ReadMap &read)
	{
		//Unload data
		UnloadData();
		
		//Allocate sectors
		sector = new Sector[sectors = read.sectors]{};
		if (sector == nullptr)
			return error.Push("Failed to allocate sectors");
		
		//Read sectors
		Sector *tsector = sector;
		for (size_t i = 0; i < sectors; i++, tsector++)
		{
			//Read sector data
			const ReadSector *fsector = read.sector[i];
			
			tsector->floor_height = fsector->floor_height;
			tsector->ceiling_height = fsector->ceiling_height;
			tsector->floor_colour = fsector->floor_colour;
			tsector->ceiling_colour = fsector->ceiling_colour;
			
			//Allocate walls
			tsector->wall = new Wall[tsector->walls = fsector->walls];
			if (tsector->wall == nullptr)
				return error.Push("Failed to allocate walls");
			
			//Read walls
			Wall *twall = tsector->wall;
			const ReadWall *fwall = fsector->wall;
			for (size_t j = 0; j < tsector->walls; j++, twall++, fwall++)
			{
				if (fwall->sector != nullind)
					twall->sector = &sector[fwall->sector];
				else
					twall->sector = nullptr;
				twall->lower_colour  = fwall->lower_colour;
				twall->middle_colour = fwall->middle_colour;
				twall->upper_colour  = fwall->upper_colour;
			}
			
			//Allocate vertices
			tsector->vertex = new Vertex[tsector->walls + 1];
			if (tsector->vertex == nullptr)
				return error.Push("Failed to allocate vertices");
			
			//Read vertices
			Vertex *tvertex = tsector->vertex;
			const size_t *fvertex = fsector->vertex;
			for (size_t j = 0; j < tsector->walls; j++)
				*tvertex++ = read.vertex[*fvertex++];
			*tvertex++ = read.vertex[fsector->vertex[0]];
		}
		
		return false;
	}
	
	//Constructor and destructor
	Map::Map()
	{
		//Initialize viewport
		SetViewport(GAME_WIDTH, GAME_HEIGHT);
		
		//Initialize view
		view.x = 0.0f;
		view.y = 0.0f;
		view.angle = pi / 2.0f;
		
		texture = new Render::Texture(executable_dir + "Data/Weapon/Shotgun/0.png");
		
		//Read data
		if (ReadData(map_data))
			return;
	}
	
	Map::~Map()
	{
		//Unload data
		UnloadData();
		
		//Delete viewport clippers
		delete texture;
	}
	
	//Map interface
	bool Map::Process(const Game::GameState &game_state)
	{
		static float xm = 0.0f, ym = 0.0f;
		
		//Move view
		view.angle = std::fmod(view.angle + game_state.turn * 3.0f * game_state.dt, pi * 2.0f);
		view.sin =  std::sin(view.angle);
		view.cos =  std::cos(view.angle);
		
		float forward_power = 0.03125f * (game_state.forward * 50.0f * 35.0f) * game_state.dt;
		xm += view.cos * forward_power;
		ym += view.sin * forward_power;
		float frict = std::pow(0.0318919214f, game_state.dt);
		xm *= frict;
		ym *= frict;
		
		view.x += xm * 35.0f * game_state.dt;
		view.y += ym * 35.0f * game_state.dt;
		
		//Bobbing
		float bob = ((xm * xm) + (ym * ym)) / 4.0f;
		if (bob > 16.0f)
			bob = 16.0f;
		static float bob_angle = 0.0f;
		if ((bob_angle += (game_state.dt * pi * 35.0f / 32.0f)) >= (pi * 2))
			bob_angle -= pi * 2.0f;
		float bob_x = bob * std::cos(bob_angle);
		float bob_y = bob * std::abs(std::sin(bob_angle));
		static float z_bob_angle = 0.0f;
		if ((z_bob_angle += (game_state.dt * pi * 2.0f / 20.0f * 35.0f)) >= (pi * 2))
			z_bob_angle -= pi * 2.0f;
		view.z = bob / 2.0f * std::sin(z_bob_angle);
		
		//Get screen data
		Render::GetScreen(&screen_data, &screen_pitch);
		//Render::DrawRect(nullptr, 0xFF00FFFF);
		
		//Draw map
		Draw();
		
		//Draw shotgun
		texture->Draw(nullptr, (view.w - 256) / 2 + bob_x, view.h - 128 + bob_y);
		
		return false;
	}
}
