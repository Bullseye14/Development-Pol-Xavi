#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if position is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& position) const
{
	return (position.x >= 0 && position.x <= (int)width &&
		position.y >= 0 && position.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& position) const
{
	uchar t = GetTileAt(position);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& position) const
{
	if (CheckBoundaries(position))
		return map[(position.y*width) + position.x];

	return INVALID_WALK_CODE;
}

Movement j1PathFinding::CheckDirection(p2DynArray<iPoint>& path) const
{
	if (path.Count() >= 2)
	{
		iPoint tile = path[0];
		iPoint next_tile = path[1];

		int x_difference = next_tile.x - tile.x;
		int y_difference = next_tile.y - tile.y;

		if (x_difference == 1 && y_difference == 1) return DOWN_RIGHT;
		else if (x_difference == 1 && y_difference == -1) return UP_RIGHT;
		else if (x_difference == -1 && y_difference == 1) return DOWN_LEFT;
		else if (x_difference == -1 && y_difference == -1) return UP_LEFT;
		else if (x_difference == 1) return RIGHT;
		else if (x_difference == -1) return LEFT;
		else if (y_difference == 1)	return DOWN;
		else if (y_difference == -1) return UP;
	}

	else return NOTMOVE;
}

Movement j1PathFinding::CheckDirectionGround(p2DynArray<iPoint>& path) const
{
	if (path.Count() >= 2)
	{
		iPoint tile = path[0];
		iPoint next_tile = path[1];

		int x_difference = next_tile.x - tile.x;
		int y_difference = next_tile.y - tile.y;

		if (x_difference == 1) return RIGHT;
		else if (x_difference == -1) return LEFT;
		else if (y_difference == 1)	return DOWN;
		else if (y_difference == -1) return UP;
	}

	else return NOTMOVE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.position == point)
			return item;
		item = item->next;
	}
	return NULL;
}

p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

PathNode::PathNode() : g(-1), h(-1), position(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& position, const PathNode* parent) : g(g), h(h), position(position), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), position(node.position), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();


	//north-east
	cell.create(position.x + 1, position.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north-west
	cell.create(position.x - 1, position.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-east
	cell.create(position.x + 1, position.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-west
	cell.create(position.x - 1, position.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// north
	cell.create(position.x, position.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(position.x, position.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(position.x + 1, position.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(position.x - 1, position.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));



	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;

	int x_distance = abs(position.x - destination.x);
	int y_distance = abs(position.y - destination.y);

	h = (x_distance + y_distance) * min(x_distance, y_distance);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
p2DynArray<iPoint>* j1PathFinding::CreatePath(iPoint& origin, iPoint& destination)
{
	BROFILER_CATEGORY("CreatePath", Profiler::Color::SlateGray)

		last_path.Clear();
	// If origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination)) {

		// We create two lists: open, close, and we add the origin tile to open, and iterate while we have tile in the open list
		PathList open, close;
		PathNode origin(0, origin.DistanceNoSqrt(destination), origin, nullptr);
		open.list.add(origin);

		while (open.list.count() > 0)
		{
			// We move the lowest score cell from open list to the closed list
			close.list.add(open.GetNodeLowestScore()->data);
			open.list.del(open.GetNodeLowestScore());

			if (close.list.end->data.position != destination)
			{
				// We fill a list of all adjancent nodes
				PathList adjancent;

				// We iterate adjancent nodes:
				close.list.end->data.FindWalkableAdjacents(adjancent);

				for (p2List_item<PathNode>* iterator = adjancent.list.start; iterator != nullptr; iterator = iterator->next)
				{
					// Ignore nodes in the closed list
					if (close.Find(iterator->data.position))
						continue;

					// If it is already in the open list, check if it is a better path (compare G)
					else if (open.Find(iterator->data.position))
					{
						PathNode tmp = open.Find(iterator->data.position)->data;
						iterator->data.CalculateF(destination);
						if (tmp.g > iterator->data.g)
						{
							// If it is a better path, Update the parent
							tmp.parent = iterator->data.parent;
						}
					}
					// If it is NOT found, calculate its F and add it to the open list
					else
					{
						iterator->data.CalculateF(destination);
						open.list.add(iterator->data);
					}
				}
				adjancent.list.clear();
			}
			else
			{
				// If we just added the destination, we are done!
				for (p2List_item<PathNode>* iterator = close.list.end; iterator->data.parent != nullptr; iterator = close.Find(iterator->data.parent->position))
				{
					// Backtrack to create the final path
					last_path.PushBack(iterator->data.position);
					if (iterator->data.parent == nullptr)
						last_path.PushBack(close.list.start->data.position);
				}

				// We use the Pathnode::parent and Flip() the path when you are finish
				last_path.Flip();
				return &last_path;
			}
		}
	}

	return nullptr;
}

