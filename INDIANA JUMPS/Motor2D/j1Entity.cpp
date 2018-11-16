#include "j1Module.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Textures.h"

j1Entity::j1Entity(int x, int y, ENTITY_TYPES type) : position(x, y), type(type)
{
}
j1Entity::~j1Entity()
{
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
