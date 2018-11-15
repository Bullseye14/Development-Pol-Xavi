#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Entity.h"
#include "j1Module.h"
#include "j1App.h"
#include "p2List.h"
#include "j1Player.h"
#include "j1Zombie_Enemy.h"
#include "j1Bird_Enemy.h"

#define MAX_ENTITIES 100
#define MAX_ENEMIES 100
#define SCREEN_SIZE 1

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

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void CleanUpEnemies();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(Entity_Type type, int x, int y);
	void CreateEntity(const EntityInfo& info);

public:
	j1Player * player_entity = nullptr;
	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	SDL_Texture* enemy_sprites = nullptr;

	pugi::xml_node entity_config;
	pugi::xml_document config_file;

	p2List<Entity*> entities_list;

	bool player_active = false;
	bool bird_active = false;
	bool zombie_active = false;

	SDL_Texture* GetEnemySprites() const;

private:
	SDL_Texture* player_sprites = nullptr;
};

#endif // __ENTITYMANAGER_H__
