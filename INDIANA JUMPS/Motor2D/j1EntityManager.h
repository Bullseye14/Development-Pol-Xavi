#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Entity.h"
#include "j1Module.h"
#include "j1App.h"
#include "p2List.h"

class j1EntityManager : public j1Module 
{
public:

	j1EntityManager() { }
	~j1EntityManager() { }

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Entity* CreateEntity(Types type);
	void DestroyEntity(Entity* entity);

public:
	Entity * player_entity = nullptr;
	
	p2List<Entity*> entities_list;

};

#endif // __ENTITYMANAGER_H__
