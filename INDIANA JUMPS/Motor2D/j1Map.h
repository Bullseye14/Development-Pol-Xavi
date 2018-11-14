#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1EntityManager.h"

#define COST_MAP 100

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct MapLayer 
{
	inline uint PathLimit(int x, int y) const;
	p2SString			name;
	uint				width = 0;
	uint				height = 0;
	uint*				data;
	float				parallaxSpeed;
	
	MapLayer() : data(NULL) { }

	~MapLayer() 
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return x + width*y;
	}

};


struct ImageLayer
{
	p2SString		name;
	SDL_Texture*	texture;
	int				offset_x;
	int				offset_y;
	int				width;
	int				height;
	fPoint			position;
	float			speed = 0;


};

// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<ImageLayer*> image_layers;
	p2List<MapLayer*>	map_layers;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	int MovementCost(int x, int y) const;

public:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadLayerImage(pugi::xml_node& node, ImageLayer* img);
	bool LoadColliders(pugi::xml_node& node);
	// TODO 3: Create a method that loads a single layer
	
public:

	MapData data;
	SDL_Rect collider;
	uint cost_so_far[COST_MAP][COST_MAP];

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded = false;

	//p2PQueue<iPoint>	frontier;
	//p2List<iPoint>		visited;
	//p2List<iPoint>		breadcrumbs;
	//p2DynArray<iPoint>	path;
	//SDL_Texture*		tile_x = nullptr;
};

#endif // __j1MAP_H__