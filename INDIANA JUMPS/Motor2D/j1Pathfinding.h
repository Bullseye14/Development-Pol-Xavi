#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2PQueue.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Entity.h"
#include "j1EntityManager.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255
#define MAP_SIZE 100
#define COST_MAP 1000


class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	bool Start();

	// Called before quitting
	bool CleanUp();

	int CreatePath(const iPoint& enemy, const iPoint& player);

	const p2DynArray<iPoint>* LastPath() const;

	void DrawPath(p2DynArray<iPoint>& path);

	void Ground(const iPoint& beginning, p2DynArray<iPoint>& path);

	void Air(const iPoint& beginning, p2DynArray<iPoint>& path);

	// Utility: returns true if the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	SDL_Texture*	path_img = nullptr;

private:

	uchar* map = nullptr;

	p2PQueue <iPoint>	frontier;
	p2List <iPoint>		visited;
	p2List <iPoint>		breadcrumbs;
	p2DynArray<iPoint>	last_path;

	uint cost_so_far[COST_MAP][COST_MAP];

};

#endif // __j1PATHFINDING_H__