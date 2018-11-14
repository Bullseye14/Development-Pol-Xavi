#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Entity.h"
#include "j1Module.h"
#include "j1App.h"
#include "p2List.h"
#include "j1Player.h"

#define MAX_ENTITIES 20

class Player;

enum Entity_Type {
	NONE,
	PLAYER,
	ZOMBIE,
	BIRD
};

class Entity;

struct EntityInfo {
	Entity_Type type = Entity_Type::NONE;
	int x, y;
};

class j1EntityManager : public j1Module 
{
public:

	j1EntityManager();
	~j1EntityManager();

	//bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update();
	bool CleanUp();

	//Entity* CreateEntity(Types type, iPoint pos);
	//void DestroyEntity(Entity* entity);
	bool AddEnemy(Entity_Type type, int x, int y);
	void CreateEntity(const EntityInfo& info);

public:
	Entity * player_entity = nullptr;
	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];
	p2List<Entity*> entities_list;

	bool player_active = false;
	bool bird_active = false;
	bool zombie_active = false;

private:
	SDL_Texture* enemy_sprites;
};

#endif // __ENTITYMANAGER_H__
