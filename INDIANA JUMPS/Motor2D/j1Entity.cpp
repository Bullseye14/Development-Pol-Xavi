#include "j1Module.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"

#include "Brofiler/Brofiler.h"
#pragma comment( lib, "Brofiler/ProfilerCore32.lib" )

j1Entity::j1Entity(int x, int y, ENTITY_TYPES type) : position(x, y), type(type)
{
}
j1Entity::~j1Entity()
{
}

bool j1Entity::Awake(pugi::xml_node& ent_config) 
{
	return true;
}

bool j1Entity::Start()
{
	return true;
}

bool j1Entity::Update(float dt)
{
	return true;
}

bool j1Entity::PostUpdate()
{
	return true;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::CleanUp()
{
	return true;
}

void j1Entity::CheckCollision()
{
	App->entity_m->player->Check_Collision();
}
