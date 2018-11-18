#ifndef __j1ZOMBIE_H__
#define __j1ZOMBIE_H__

#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"


struct SDL_Texture;
//struct Collider;

class j1Zombie : public j1Entity
{
public:
	j1Zombie(int x, int y, ENTITY_TYPES type);

	virtual ~j1Zombie();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;

	//void Move(p2DynArray<iPoint>& path, float dt);

public:

	Animation walk_left;
	Animation walk_right;
	Animation die_left;
	Animation die_right;

public:
	fPoint initialPosition;

	iPoint margin;
	iPoint colliderSize;

	uint c = 0;

	bool move = false;
	bool die = false;

	bool path_created = false;

};
#endif

