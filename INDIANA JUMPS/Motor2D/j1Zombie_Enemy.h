#ifndef __ZOMBIEENEMY_H__
#define __ZOMBIEENEMY_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"


class j1Zombie_Enemy : public Entity
{
public:
	j1Zombie_Enemy(int x, int y);
	~j1Zombie_Enemy();

	bool Awake();
	void MoveEntity(float dt);
	void Draw(float dt);

private:
	Animation walk_left;
	Animation walk_right;
	Animation dying;

	uint c = 0;

	bool move = false;
	bool die = false;
	
	fPoint position;

};

#endif // __ZOMBIEENEMY_H__
