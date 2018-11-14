#ifndef __BIRDENEMY_H__
#define __BIRDENEMY_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

class j1Bird_Enemy : public Entity
{
public:
	j1Bird_Enemy();
	~j1Bird_Enemy();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	fPoint initial_pos;

};

#endif // __BIRDENEMY_H__
