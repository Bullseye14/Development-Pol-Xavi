#include "j1Zombie.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"

#include "Brofiler/Brofiler.h"
#pragma comment( lib, "Brofiler/ProfilerCore32.lib" )

j1Zombie::j1Zombie(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::ZOMBIE) 
{
	walk_left.PushBack({ 0,0,64,64 });
	walk_left.PushBack({ 64,0,64,64 });
	walk_left.PushBack({ 0,64,64,64 });
	walk_left.PushBack({ 64,64,64,64 });
	walk_left.PushBack({ 0,128,64,64 });
	walk_left.PushBack({ 64,128,64,64 });
	walk_left.PushBack({ 0,192,64,64 });
	walk_left.PushBack({ 64,192,64,64 });
	walk_left.PushBack({ 0,256,64,64 });
	walk_left.PushBack({ 64,256,64,64 });
	walk_left.loop = true;
	walk_left.speed = 0.1f;

	walk_right.PushBack({ 192,0,64,64 });
	walk_right.PushBack({ 128,0,64,64 });
	walk_right.PushBack({ 192,64,64,64 });
	walk_right.PushBack({ 128,64,64,64 });
	walk_right.PushBack({ 192,128,64,64 });
	walk_right.PushBack({ 128,128,64,64 });
	walk_right.PushBack({ 192,192,64,64 });
	walk_right.PushBack({ 128,192,64,64 });
	walk_right.PushBack({ 192,256,64,64 });
	walk_right.PushBack({ 128,256,64,64 });
	walk_right.loop = true;
	walk_right.speed = 0.1f;

	die_left.PushBack({ 256,0,64,64 });
	die_left.PushBack({ 320,0,64,64 });
	die_left.PushBack({ 256,64,64,64 });
	die_left.PushBack({ 320,64,64,64 });
	die_left.PushBack({ 256,128,64,64 });
	die_left.PushBack({ 320,128,64,64 });
	die_left.PushBack({ 256,192,64,64 });
	die_left.PushBack({ 320,192,64,64 });
	die_left.PushBack({ 256,256,64,64 });
	die_left.PushBack({ 320,256,64,64 });
	die_left.PushBack({ 256,320,64,64 });
	die_left.PushBack({ 320,320,64,64 });
	die_left.loop = true;
	die_left.speed = 0.1f;

	die_right.PushBack({ 448,0,64,64 });
	die_right.PushBack({ 384,0,64,64 });
	die_right.PushBack({ 448,64,64,64 });
	die_right.PushBack({ 384,64,64,64 });
	die_right.PushBack({ 448,128,64,64 });
	die_right.PushBack({ 384,128,64,64 });
	die_right.PushBack({ 448,192,64,64 });
	die_right.PushBack({ 384,192,64,64 });
	die_right.PushBack({ 448,256,64,64 });
	die_right.PushBack({ 384,256,64,64 });
	die_right.PushBack({ 448,320,64,64 });
	die_right.PushBack({ 384,320,64,64 });
	die_right.loop = true;
	die_right.speed = 0.1f;

	initialPosition.x = position.x = x;
	initialPosition.y = position.y = y;
}

j1Zombie::~j1Zombie()
{}

bool j1Zombie::Awake(pugi::xml_node& config) 
{
	pugi::xml_node zombie = config.child("entities").child("zombie");

	return true;
}

bool j1Zombie::Start() 
{
	graphics = App->tex->Load("textures/Enemies_Spritesheet.png");

	animation = &walk_left;

	playerHitbox = App->collision->AddCollider({ (int)position.x, (int)position.y, 32, 64 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	speed.x = 20;
	speed.y = 0;

	return true;
}

bool j1Zombie::Update(float dt) 
{
	BROFILER_CATEGORY("Zombie Entity Update", Profiler::Color::AliceBlue);
	
	playerHitbox->SetPos(position.x + 16, position.y);
	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

	CheckCollision();

	iPoint EnemyPos = { (int)initialPosition.x + 32, (int)initialPosition.y };
	iPoint PlayerPos{ (int)App->entity_m->player->position.x, (int)App->entity_m->player->position.y};

	App->pathfinding->CreatePath(EnemyPos, PlayerPos);
	App->pathfinding->Air(PlayerPos, path);

	position = initialPosition;

	if ((abs(App->entity_m->player->position.x - EnemyPos.x) < 400) && !move)
	{
		c = 0;

		App->pathfinding->CreatePath(EnemyPos, PlayerPos);
		App->pathfinding->Ground(PlayerPos, path);

		move = true;
	}

	if (move)
	{
		iPoint objective = { path[c].x,path[c].y };

		animation = &walk_left;

		if (EnemyPos.x < objective.x)
		{
			initialPosition.x += speed.x * dt;

			if (EnemyPos.x >= objective.x)
			{
				c++;
				move = false;
			}
		}

		else
		{
			initialPosition.x -= speed.x*dt;
			animation = &walk_right;

			if (EnemyPos.x <= objective.x)
			{
				c++;
				move = false;
			}
		}
	}
	else
	{
		animation = &walk_left;
	}

	if (abs(App->entity_m->player->position.x - EnemyPos.x) >= 400)
	{
		move = false;
	}

	position.x += speed.x;
	position.y += speed.y;

	return true;
}

bool j1Zombie::CleanUp()
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr)
		playerHitbox->to_delete = true;

	return true;
}

void j1Zombie::OnCollision(Collider * c1, Collider * c2)
{
	if (c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_WALL)
	{
		// touches from above
		if ((c2->rect.y) > (c1->rect.y + c1->rect.h - 10))
		{
			from_up = true;
		}
		// touches from right
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10))
		{
			from_right = true;
		}
		// touches from left
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10))
		{
			from_left = true;
		}
	}

	if (c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_DEATH || c2->type == COLLIDER_SLIDE) 
	{
		die = true;
	}

}

void j1Zombie::CheckCollision() 
{
	if (!from_up)
	{
		speed.y = 2.5f;
	}

	if (from_left)						// Colliding from left
	{
		if (speed.x < 0) { speed.x = 0; }
	}

	if (from_right)						// Colliding from right
	{
		if (speed.x > 0) { speed.x = 0; }
	}
}