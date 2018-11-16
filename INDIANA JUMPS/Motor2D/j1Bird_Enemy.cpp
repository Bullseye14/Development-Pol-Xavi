#include "j1Bird_Enemy.h"


j1Bird_Enemy::j1Bird_Enemy(int x, int y) : Entity(x, y)
{
	App->entity_m->bird_active = true;

	fly_right.PushBack({ 0,320,64,64 });
	fly_right.PushBack({ 64,320,64,64 });
	fly_right.PushBack({ 128,320,64,64 });
	fly_right.PushBack({ 192,320,64,64 });
	fly_right.loop = true;

	fly_left.PushBack({ 0,384,64,64 });
	fly_left.PushBack({ 64,384,64,64 });
	fly_left.PushBack({ 128,384,64,64 });
	fly_left.PushBack({ 192,384,64,64 });
	fly_left.loop = true;

	current_animation = &fly_right;

	collider = App->collision->AddCollider({ (int)position.x+16,(int)position.y+16,32,32 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	initial_pos = initial_position.x;

}

bool j1Bird_Enemy::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	config = config.child("entities").child("bird");
	// TODO
	// Valors del Bird per llegir del config file

	return true;
}

j1Bird_Enemy::~j1Bird_Enemy()
{
	App->tex->UnLoad(spritesheet);
}

void j1Bird_Enemy::MoveEntity(float dt)
{
	position = initial_position;

	iPoint EnemyPos = { (int)initial_position.x + 32, (int)initial_position.y };
	iPoint PlayerPos{ (int)App->entity_m->player_entity->position.x + 30, (int)App->entity_m->player_entity->position.y + 46 };

	if ((abs(App->entity_m->player_entity->position.x - EnemyPos.x) < 400) && !move)
	{
		c = 0;

		App->pathfinding->CreatePath(EnemyPos, PlayerPos);
		App->pathfinding->Air(PlayerPos, path);

		move = true;
	}

	if (move)
	{
		iPoint objective = { path[c].x,path[c].y };

		current_animation = &fly_left;

		if (EnemyPos.x < objective.x)
		{
			initial_position.x += speed.x*dt;

			if (EnemyPos.x >= objective.x)
			{
				c++;
				move = false;
			}
		}

		else
		{
			initial_position.x -= speed.x*dt;
			current_animation = &fly_right;

			if (EnemyPos.x <= objective.x)
			{
				c++;
				move = false;
			}
		}

		if (EnemyPos.y < objective.y)
		{
			initial_position.y += speed.y*dt;

			if (EnemyPos.y >= objective.y)
			{
				c++;
				move = false;
			}
		}

		else
		{
			initial_position.y -= speed.y*dt;

			if (EnemyPos.y < objective.y)
			{
				c++;
				move = false;
			}
		}
	}
	else
	{
		current_animation = &fly_left;
	}

	if (abs(App->entity_m->player_entity->position.x - EnemyPos.x) >= 400)
	{
		move = false;
	}
}

void j1Bird_Enemy::Draw(float dt)
{
	current_animation->speed = 10.f*dt;
	collider->SetPos(position.x + 16, position.y + 16);
	App->render->Blit(App->entity_m->GetEnemySprites(), position.x, position.y, &(current_animation->GetCurrentFrame()));
}