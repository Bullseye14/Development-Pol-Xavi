#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Map.h"

void j1PathFinding::SetMap(uint w, uint h, uchar* data) 
{
	this->w = w;
	this->h = h;

	RELEASE_ARRAY(map);

	map = new uchar[w*h];
	memcpy(map, data, w*h);
}

bool j1PathFinding::FloorContact(iPoint position) const
{
	return !IsWalkable({ position.x, position.y + 1 });
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& position) const
{
	bool ret = false;

	int coordinates = (position.y * w) + position.x;
	
	if (position.x < w && map[coordinates] == 1 && position.x >= 0 && position.y >= 0 && position.y < h - 1) 
	{
		ret = true;
	}

	return ret;
}

void j1PathFinding::NewPath(p2DynArray <iPoint>& new_path)
{
	new_path.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	memset(cost_so_far, 0, sizeof(uint) * MAP_SIZE * MAP_SIZE);
	memset(falls, 0, sizeof(uint) * MAP_SIZE * MAP_SIZE);
}

bool j1PathFinding::DoPath(Entity* enemy, Entity* player, p2DynArray <iPoint> & path) 
{
	bool ret = false;

	NewPath(path);

	//Define origin and destination coords
	iPoint pos_initial = App->map->WorldToMap(enemy->position.x, enemy->position.y);
	frontier.Push(pos_initial, 0);
	visited.add(pos_initial);
	breadcrumbs.add(pos_initial);
	iPoint destination = App->map->WorldToMap(player->position.x, player->position.y);

	if (IsWalkable(destination) && IsWalkable(pos_initial) && (pos_initial.y <= destination.y))
	{
		while (visited.find(destination) == -1)
		{
			iPoint curr;
			if (frontier.Pop(curr))
			{

				iPoint neighbors[4];
				neighbors[0].create(curr.x + 0, curr.y + 1); 
				neighbors[1].create(curr.x - 1, curr.y + 0); 
				neighbors[2].create(curr.x + 1, curr.y + 0); 
				neighbors[3].create(curr.x + 0, curr.y - 1); 

				int max_neighbors = 4;

				for (uint i = 0; i < max_neighbors; ++i)
				{
					if (!IsWalkable(neighbors[i])) //If not walkable ignore
						continue;

					int distance = neighbors[i].DistanceNoSqrt(destination);

					int newCost = cost_so_far[curr.x][curr.y] + 1; //g
					if (cost_so_far[neighbors[i].x][neighbors[i].y] == NULL || newCost < cost_so_far[neighbors[i].x][neighbors[i].y])
					{
						cost_so_far[neighbors[i].x][neighbors[i].y] = newCost;

						//Set falling value
						if (curr.x == neighbors[i].x)
							falls[neighbors[i].x][neighbors[i].y] = falls[curr.x][curr.y] + (falls[curr.x][curr.y] % 2 == 0) ? 2 : 1;
						else if (curr.y == neighbors[i].y)
							falls[neighbors[i].x][neighbors[i].y] = falls[curr.x][curr.y] + 1;
						if (FloorContact({ neighbors[i].x , neighbors[i].y }))
							falls[neighbors[i].x][neighbors[i].y] = 0;

						//Add to visited or update breadcrumb
						if (visited.find(neighbors[i]) == -1)
						{
							breadcrumbs.add(curr);
							visited.add(neighbors[i]);
							if (neighbors[i] != destination)
							{
								frontier.Push(neighbors[i], newCost + (distance * 10));
							}
						}
						else
						{
							breadcrumbs.At(visited.find(neighbors[i]))->data = curr;
						}
					}
				}
			}
			else
				break;
		}

		//Calculate final path
		if (visited.find(destination) != -1)
		{
			ret = true;
			path.PushBack(destination);
			iPoint cameFrom = breadcrumbs.At(visited.find(destination))->data;
			path.PushBack(cameFrom);
			while (cameFrom != visited.start->data)
			{
				cameFrom = breadcrumbs.At(visited.find(cameFrom))->data;
				path.PushBack(cameFrom);
			}
		}
		path.Flip();
	}

	return ret;
}
