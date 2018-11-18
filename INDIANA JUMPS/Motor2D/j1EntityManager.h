#ifndef __J1ENEMIES_H__
#define __J1ENEMIES_H__
#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "j1App.h"

#define MAX_ENEMIES 50
class j1Entity;
class j1Player;
class j1Hook;
struct SDL_Texture;

enum ENTITY_TYPES
{
	NONE,
	PLAYER,
	BIRD
};
struct EnemyInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NONE;
	iPoint position;
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
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	j1Entity* CreateEntity(ENTITY_TYPES type, int x = 0, int y = 0);
	j1Entity* entities[MAX_ENEMIES];
	void OnCollision(Collider* c1, Collider* c2);
	void CreatePlayer();
	void AddEnemy(int x, int y, ENTITY_TYPES type);
private:
	void SpawnEnemy(const EnemyInfo& info);
public:
	p2List<j1Entity*>	entity_list;
	j1Player*			player = nullptr;
	pugi::xml_document config_file;
	pugi::xml_node entities_config;
private:
	EnemyInfo			queue[MAX_ENEMIES];
};
#endif // __J1ENEMIES_H__ 
