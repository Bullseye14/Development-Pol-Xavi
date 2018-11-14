#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Animation.h"
#include "p2Point.h"
#include "j1Collision.h"
#include "p2DynArray.h"

struct SDL_Texture;

class Entity 
{
protected:
	Animation* current_animation = nullptr;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual bool Awake(pugi::xml_node&) { return true; }

	virtual void Draw(float dt) { }

	virtual void OnCollision() { }

	virtual void MoveEntity(float dt) {};

	SDL_Texture* spritesheet = nullptr;

	fPoint position;
	fPoint collider_position;
	fPoint initial_position;
	fPoint speed;

	Collider* collider = nullptr;

	p2DynArray<iPoint> path;

	int initial_pos;
	int x_scale;
	int y_scale;

	bool flip;

};

#endif // __ENTITY_H__
