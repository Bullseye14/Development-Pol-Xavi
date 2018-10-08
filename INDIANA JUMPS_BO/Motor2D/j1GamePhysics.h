#ifndef __j1GAMEPHYSICS_H__
#define __j1GAMEPHYSICS_H__

#include "SDL/include/SDL.h"
#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#define MAX_COLLIDERS 50

enum COLLIDER_TYPE {PLAYER, PLATFORM, DEATH};

class Collider{
public:
	SDL_Rect rect;
	COLLIDER_TYPE coltype;
	//Collider(SDL_Rect *r, COLLIDER_TYPE coltype) : rect(r), coltype(type) {}
};

class j1GamePhysics : public j1Module
{
public:

	j1GamePhysics();

	// Destructor
	virtual ~j1GamePhysics();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	bool PreUpdate(float dt);
	bool Update(float dt);

	void DebugDraw() const;

	Collider* AddCollider(SDL_Rect *rect, const COLLIDER_TYPE coltype);

	Collider* checkCollider(Collider checkcol, COLLIDER_TYPE coltype) const;

private:

	Collider* colliders[MAX_COLLIDERS];

public:

	p2List<Collider*>	colliders;


private:
	pugi::xml_document	physics_file;
};
#endif