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

	name.create("entities");
}

bool j1EntityManager::Awake(pugi::xml_node& config) 
{
	config_file.load_file("config.xml");
	entity_config = config;

	return true;
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Start() 
{
	enemy_sprites = App->tex->Load("textures/Enemy_Spritesheet.png");
	player_sprites = App->tex->Load("textures/Spritesheet.png");

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

bool j1EntityManager::PreUpdate() 
{
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

bool j1EntityManager::Update(float dt) 
{
	if (dt < 1) 
	{
		for (uint i = 0; i < MAX_ENEMIES; ++i) 
		{
			if (entities[i] != nullptr) 
			{
				entities[i]->MoveEntity(dt);
				entities[i]->Draw(dt);
				entities[i]->Awake(entity_config);
			}
			if (player_entity != nullptr) 
			{
				player_entity->MoveEntity(dt);
				player_entity->Draw(dt);
			}
		}
	}
	
	return true;
}

bool j1EntityManager::PostUpdate() 
{
	for (uint i = 0; i < MAX_ENEMIES; ++i) 
	{
		if (entities[i] != nullptr) 
		{
			if ((abs((int)App->render->camera.y) + SCREEN_HEIGHT + SPAWN_MARGIN) < entities[i]->position.y) 
			{
				//delete entities[i];
				entities[i] = nullptr;
			}
		}
	}

	return true;
}

bool j1EntityManager::CleanUp() 
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
		if (queue[i].type != NONE)
		{
			queue[i].type = NONE;
		}
	}

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

void j1EntityManager::CleanUpEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != Entity_Type::NONE)
		{
			queue[i].type = Entity_Type::NONE;
		}
	}
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2) 
{

}

bool j1EntityManager::Load(pugi::xml_node& data) 
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const 
{
	return true;
}

SDL_Texture* j1EntityManager::GetEnemySprites() const { return enemy_sprites; }

