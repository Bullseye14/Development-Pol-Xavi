#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Map.h"

j1PathFinding::j1PathFinding() 
{
}

j1PathFinding::~j1PathFinding()
{
}

bool j1PathFinding::Start() 
{
	path_img = App->tex->Load("texture/path.png");
	
	return true;
}

bool j1PathFinding::CleanUp() 
{
	LOG("Freeing pathfinding");

	frontier.Clear();
	breadcrumbs.clear();
	visited.clear();
	last_path.Clear();

	return true;
}

bool j1PathFinding::IsWalkable(const iPoint& position) const 
{
	if (position.x >= 0 && position.x < App->map->data.width
		&& position.y >= 0 && position.y < App->map->data.height) 
	{
		return true;
	}
	else return false;
}

const p2DynArray<iPoint>* j1PathFinding::LastPath() const 
{
	return &last_path;
}

void j1PathFinding::DrawPath(p2DynArray<iPoint>& finding) 
{
	for (uint i = 0; i < finding.Count(); ++i) 
	{
		iPoint position = App->map->MapToWorld(finding[i].x, finding[i].y);
		App->render->Blit(path_img, position.x, position.y);
	}
}

void j1PathFinding::Ground(const iPoint& beginning, p2DynArray<iPoint>& path)
{
	path.Clear();

	iPoint objective = { beginning.x,beginning.y };
	iPoint curr = objective;

	path.PushBack(curr);

	while (curr != breadcrumbs.start->data && visited.find(objective) != -1) 
	{
		curr = breadcrumbs[visited.find(curr)];
		path.PushBack(curr);
	}

	path.Flip();
}

void j1PathFinding::Air(const iPoint& beginning, p2DynArray<iPoint>& path) 
{
	path.Clear();

	iPoint objective = { beginning.x,beginning.y };
	iPoint curr = objective;

	path.PushBack(curr);

	while (curr != breadcrumbs.start->data&&visited.find(objective) != -1)
	{
		curr = breadcrumbs[visited.find(curr)];
		path.PushBack(curr);
	}

	path.Flip();
}

int j1PathFinding::CreatePath(const iPoint& beginning, const iPoint& objective) 
{
	frontier.Clear();
	breadcrumbs.clear();
	visited.clear();

	int ret = 0;

	if (ret != -1) 
	{
		iPoint curr;

		iPoint goal = App->map->WorldToMap(objective.x, objective.y);

		frontier.Push(App->map->WorldToMap(beginning.x, beginning.y), 0);

		while (frontier.Count() != 0)
		{
			if (curr == goal)
			{
				break;
			}
			if (frontier.Pop(curr))
			{
				iPoint neighbors[4];
				neighbors[0].create(curr.x + 1, curr.y + 0);
				neighbors[1].create(curr.x + 0, curr.y + 1);
				neighbors[2].create(curr.x - 1, curr.y + 0);
				neighbors[3].create(curr.x + 0, curr.y - 1);


				for (uint i = 0; i < 4; ++i)
				{
					uint point_dist = neighbors[i].DistanceTo(goal);

					if (App->map->MovementCost(neighbors[i].x, neighbors[i].y) > 0 && App->entity_m->zombie_entity)
					{
						if (visited.find(neighbors[i]) == -1 && breadcrumbs.find(neighbors[i]) == -1)
						{
							cost_so_far[neighbors[i].x][neighbors[i].y] = point_dist;
							frontier.Push(neighbors[i], point_dist);
							visited.add(neighbors[i]);
							breadcrumbs.add(curr);
						}
					}
					if (App->map->MovementCost(neighbors[i].x, neighbors[i].y) >= 0 && App->entity_m->bird_entity)
					{
						if (visited.find(neighbors[i]) == -1 && breadcrumbs.find(neighbors[i]) == -1)
						{
							cost_so_far[neighbors[i].x][neighbors[i].y] = point_dist;
							frontier.Push(neighbors[i], point_dist);
							visited.add(neighbors[i]);
							breadcrumbs.add(curr);
						}
					}
				}
			}
		}
	}

	return ret;
}