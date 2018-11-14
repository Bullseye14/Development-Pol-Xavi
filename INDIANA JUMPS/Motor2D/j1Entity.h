#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

enum Types 
{
	PLAYER = 0,
	ZOMBIE,
	BIRD,
	NONE
};

class Entity 
{
protected:
	int type;
	Animation* current_animation = NULL;
	Collider* collider;

public:
	Entity(int x, int y);
	virtual ~Entity();

	fPoint position;
	char* name;

public:
	

};

#endif // __ENTITY_H__
