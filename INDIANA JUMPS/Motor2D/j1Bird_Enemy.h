#ifndef __BIRDENEMY_H__
#define __BIRDENEMY_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"


class j1Bird_Enemy : public Entity
{
public:
	j1Bird_Enemy(int x, int y);
	~j1Bird_Enemy();

	bool Awake();
	void MoveEntity(float dt);
	void Draw(float dt);

private:
	Animation fly_left;
	Animation fly_right;

	uint c = 0;

	bool move = false;
	bool die = false;

	fPoint position;

};

#endif // __BIRDENEMY_H__
