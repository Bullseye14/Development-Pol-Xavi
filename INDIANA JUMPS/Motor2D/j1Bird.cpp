#include "j1Bird.h"
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


j1Bird::j1Bird(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::BIRD)
{
	fly_right.PushBack({ 0,320,64,64 });
	fly_right.PushBack({ 64,320,64,64 });
	fly_right.PushBack({ 128,320,64,64 });
	fly_right.PushBack({ 192,320,64,64 });
	fly_right.loop = true;
	fly_right.speed = 0.1f;

	fly_left.PushBack({ 0,384,64,64 });
	fly_left.PushBack({ 64,384,64,64 });
	fly_left.PushBack({ 128,384,64,64 });
	fly_left.PushBack({ 192,384,64,64 });
	fly_left.loop = true;
	fly_left.speed = 0.1f;

	initialPosition.x = position.x = x;
	initialPosition.y = position.y = y;
}

j1Bird::~j1Bird()
{
}

bool j1Bird::Start()
{
	graphics = App->tex->Load("textures/Enemies_Spritesheet.png");

	animation = &fly_left;

	playerHitbox = App->collision->AddCollider({ (int)position.x, (int)position.y, 40, 40 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	speed.x = 40;
	speed.y = 40;

	return true;
}

bool j1Bird::Update(float dt)
{
	playerHitbox->SetPos(position.x + 10, position.y + 13);
	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());
	
	iPoint EnemyPos = { (int)initialPosition.x + 32, (int)initialPosition.y };
	iPoint PlayerPos{ (int)App->entity_m->player->position.x + 30, (int)App->entity_m->player->position.y + 46 };
	
	App->pathfinding->CreatePath(EnemyPos, PlayerPos);
	App->pathfinding->Air(PlayerPos, path);
	
	position = initialPosition;

	if ((abs(App->entity_m->player->position.x - EnemyPos.x) < 400) && !move)
	{
		c = 0;

		App->pathfinding->CreatePath(EnemyPos, PlayerPos);
		App->pathfinding->Air(PlayerPos, path);

		move = true;
	}

	if (move)
	{
		iPoint objective = { path[c].x,path[c].y };

		animation = &fly_left;

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
			animation = &fly_right;

			if (EnemyPos.x <= objective.x)
			{
				c++;
				move = false;
			}
		}

		if (EnemyPos.y < objective.y)
		{
			initialPosition.y += speed.y*dt;

			if (EnemyPos.y >= objective.y)
			{
				c++;
				move = false;
			}
		}

		else
		{
			initialPosition.y -= speed.y*dt;

			if (EnemyPos.y < objective.y)
			{
				c++;
				move = false;
			}
		}
	}
	else
	{
		animation = &fly_left;
	}

	if (abs(App->entity_m->player->position.x - EnemyPos.x) >= 400)
	{
		move = false;
	}

	return true;
}

bool j1Bird::CleanUp()
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr)
		playerHitbox->to_delete = true;

	return true;
}

void j1Bird::OnCollision(Collider * col_1, Collider * col_2)
{
}
