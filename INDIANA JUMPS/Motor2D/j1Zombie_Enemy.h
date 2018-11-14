#ifndef __ZOMBIEENEMY_H__
#define __ZOMBIEENEMY_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

class j1Zombie_Enemy : public Entity
{
public:
	j1Zombie_Enemy();
	~j1Zombie_Enemy();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	fPoint initial_pos;

};

#endif // __ZOMBIEENEMY_H__
