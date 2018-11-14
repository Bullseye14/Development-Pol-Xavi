#include "j1Entity.h"
#include "j1EntityManager.h"

Entity::Entity(int x, int y) {

}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}
