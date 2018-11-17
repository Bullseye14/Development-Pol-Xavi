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

class j1Zombie : public j1Entity 
{
public:
	j1Zombie(int x, int y, ENTITY_TYPES type);

	virtual ~j1Zombie();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	void CheckCollision();

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

	bool from_up;
	bool from_right;
	bool from_left;
};

#endif // __j1ZOMBIE_H__
