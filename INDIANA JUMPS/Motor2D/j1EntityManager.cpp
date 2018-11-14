#include "Globals.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Zombie_Enemy.h"
#include "j1Bird_Enemy.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"


#define SPAWN_MARGIN 100

j1EntityManager::j1EntityManager()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
		entities[i] = nullptr;
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Start() 
{
	
	enemy_sprites = App->tex->Load("textures/Enemy_Spritesheet.png");

	if (player_entity == nullptr)
	{
		player_entity = new j1Player(5, 10);
		player_entity->Awake(entity_config);
		player_entity->Start();
	}
	else
		player_entity->Start();

	return true;
}

bool j1EntityManager::PreUpdate() {
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != Entity_Type::NONE)
		{
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{

				CreateEntity(queue[i]);
				queue[i].type = Entity_Type::NONE;

			}
		}
	}
	return true;
}

bool j1EntityManager::Update() 
{
	return true;
}

bool j1EntityManager::CleanUp() 
{
	return true;
}

/*Entity* j1EntityManager::CreateEntity(Types type, iPoint pos) 
{
	static_assert(Types::NONE == 3, "code needs update");
	Entity* ret = nullptr;
	
	switch (type) 
	{
	case Types::PLAYER:
		ret = new j1Player();
		player_entity = ret;
		break;

	case Types::ZOMBIE:
		ret = new j1Zombie_Enemy();
		break;

	case Types::BIRD:
		ret = new j1Bird_Enemy();
		break;
	}

	if (ret != nullptr) 
	{
		entities_list.add(ret);
	}

	return ret;
}*/

//void j1EntityManager::DestroyEntity(Entity* entity) 

bool j1EntityManager::AddEnemy(Entity_Type type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == Entity_Type::NONE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;

			ret = true;
			break;
		}
	}

	return ret;
}


void j1EntityManager::CreateEntity(const EntityInfo& info)
{
	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENTITIES; ++i);

	if (i != MAX_ENTITIES)
	{
		switch (info.type)
		{
		case Entity_Type::PLAYER:
			entities[i] = new j1Player(info.x, info.y);
			break;
		case Entity_Type::ZOMBIE:
			entities[i] = new j1Zombie_Enemy(info.x, info.y);
			break;
		case Entity_Type::BIRD:
			entities[i] = new j1Bird_Enemy(info.x, info.y);
			break;
		}
	}
}

