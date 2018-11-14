#include "j1Entity.h"
#include "j1EntityManager.h"

Entity::Entity(int x, int y) : position(x, y), initial_position(x, y), collider_position(0, 0)
{

}

bool Entity::Awake(pugi::xml_node& data) 
{
	return true;
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(float dt) 
{
	
}

void Entity::OnCollision() 
{

}