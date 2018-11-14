#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "p2PQueue.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255
#define MAP_SIZE 100


class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Path
	bool DoPath(Entity* enemy, Entity* player, p2DynArray<iPoint> & path);
	
	// Map
	void SetMap(uint w, uint h, uchar* data);
	
	// Returns true if it's walkable
	bool IsWalkable(const iPoint & position) const;
	
	// Resets the path
	void NewPath(p2DynArray<iPoint> & new_path);
	
	// Returns true if it's on the floor
	bool FloorContact(iPoint position) const;

private:

	uint w = 0; 
	uint h = 0;
	uchar* map = nullptr;

	p2PQueue <iPoint>	frontier;
	p2List <iPoint>		visited;
	p2List <iPoint>		breadcrumbs;

	uint cost_so_far[MAP_SIZE][MAP_SIZE];
	uint falls[MAP_SIZE][MAP_SIZE];

};

#endif // __j1PATHFINDING_H__
