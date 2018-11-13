#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Zombie_Enemy.h"
#include "j1Bird_Enemy.h"

Entity* j1EntityManager::CreateEntity(Types type) 
{
	static_assert(Types::NONE == 3, "code needs update");
	Entity* ret = nullptr;
	
	switch (type) 
	{
	case Types::PLAYER:
		ret = new j1Player();
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
}