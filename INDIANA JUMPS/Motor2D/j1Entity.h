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
	fPoint position;
	char* name;

public:
	Entity::Entity(Types type, fPoint initial_pos) : type(type), position(initial_pos) { };

};

#endif // __ENTITY_H__
