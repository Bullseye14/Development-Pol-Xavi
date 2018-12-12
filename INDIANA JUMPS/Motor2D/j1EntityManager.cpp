#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Bird.h"
#include "j1Zombie.h"
#include "j1Coin.h"

#include "Brofiler/Brofiler.h"
#pragma comment ( lib, "Brofiler/ProfilerCore32.lib" )

j1EntityManager::j1EntityManager()
{
	name.create("EntityManager");
}
j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	config_file.load_file("config.xml");
	entities_config = config_file.child("config").child("entity");
	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	j1Entity* ent = nullptr;
	p2List_item<j1Entity*>* iterator;
	iterator = entity_list.start;
	for (iterator; iterator != nullptr && ret == true; iterator = iterator->next)
	{
		ent = iterator->data;
		ret = ent->Start();
	}
	return true;
}
bool j1EntityManager::PreUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NONE)
		{
			SpawnEnemy(queue[i]);
			queue[i].type = ENTITY_TYPES::NONE;
		}
	}
	return true;
}
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::MediumVioletRed);
	
	bool ret = true;
	j1Entity* ent = nullptr;
	p2List_item<j1Entity*>* iterator;
	iterator = entity_list.start;
	for (iterator; iterator != nullptr && ret == true; iterator = iterator->next)
	{
		ent = iterator->data;
		ret = ent->Update(dt);
	}

	return true;
}
bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("Entity Manager PostUpdate", Profiler::Color::Gray);
	
	bool ret = true;
	j1Entity* ent = nullptr;
	p2List_item<j1Entity*>* iterator;
	iterator = entity_list.start;
	for (iterator; iterator != nullptr && ret == true; iterator = iterator->next)
	{
		ent = iterator->data;
		ret = ent->PostUpdate();
	}
	return true;
}
bool j1EntityManager::CleanUp()
{
	p2List_item<j1Entity*>* iterator;
	iterator = entity_list.start;
	for (iterator; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->CleanUp();
		RELEASE(iterator->data);
	}
	player = nullptr;
	entity_list.clear();
	return true;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	p2List_item<j1Entity*>* iterator;
	iterator = entity_list.start;
	for (iterator; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->OnCollision(c1, c2);
		iterator->data->CheckCollision();
	}
}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(PLAYER);
	player->Awake(entities_config);
}

j1Entity* j1EntityManager::CreateEntity(ENTITY_TYPES type, int x, int y)
{
	j1Entity* new_entity = nullptr;
	
	if(type == PLAYER)
	{
		new_entity = new j1Player(x, y, type);

		if (new_entity != nullptr)
		{
			entity_list.add(new_entity);
		}
	}

	return new_entity;
}

void j1EntityManager::DestroyEntity(j1Entity * entity)
{
		p2List_item<j1Entity*>* iterator;
		iterator = entity_list.start;
		for (iterator; iterator != nullptr; iterator = iterator->next)
		{
			if (iterator->data == entity)
			{
				entity_list.del(iterator);
				RELEASE(iterator->data);
				break;
			}
		}
}

void j1EntityManager::AddEnemy(int x, int y, ENTITY_TYPES type)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPES::NONE)
		{
			queue[i].type = type;
			queue[i].position.x = x;
			queue[i].position.y = y;
			break;
		}
	}
}
void j1EntityManager::SpawnEnemy(const EnemyInfo& info)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NONE)
		{
			j1Entity* entity;
			if (queue[i].type == BIRD) {
				entity = new j1Bird(info.position.x, info.position.y, info.type);
			}

			else if (queue[i].type == ZOMBIE) {
				entity = new j1Zombie(info.position.x, info.position.y, info.type);
			}

			else if (queue[i].type == COIN) {
				entity = new j1Coin(info.position.x, info.position.y, info.type);
			}

			entity_list.add(entity);
			entity->Start();
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	if (player != nullptr)
	{
		player->Load(data);
	}
	return true;
}
bool j1EntityManager::Save(pugi::xml_node& data) const
{
	if (player != nullptr)
	{
		player->Save(data);
	}
	return true;
}