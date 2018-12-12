#ifndef __j1Coin_H__
#define __j1Coin_H__

#include "j1App.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "Animation.h"
#include "p2DynArray.h"

struct SDL_Texture;

class j1Coin : public j1Entity 
{
public:

	j1Coin(int x, int y, ENTITY_TYPES type);
	virtual ~j1Coin();

	bool Awake(pugi::xml_node&) { return true; }
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool taken = false;

	Animation idle;
};

#endif // __j1Coin_H__