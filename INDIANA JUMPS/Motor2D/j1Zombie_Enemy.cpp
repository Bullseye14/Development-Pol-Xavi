#include "j1Zombie_Enemy.h"

j1Zombie_Enemy::j1Zombie_Enemy(int x, int y) : Entity(x,y) 
{
	App->entity_m->zombie_active = true;

	walk_left.PushBack({ 0,0,0,0 });

	walk_right.PushBack({ 0,0,0,0 });

	dying.PushBack({ 0,0,0,0 });

	current_animation = &walk_left;

	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,32,64 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	initial_pos = initial_position.x;

}

bool j1Zombie_Enemy::Awake() 
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	config = config.child("entities").child("zombie");
	// TODO
	// Valors del Zombie per llegir del config file

	return true;
}

j1Zombie_Enemy::~j1Zombie_Enemy() 
{
	App->tex->UnLoad(spritesheet);
}

void j1Zombie_Enemy::MoveEntity(float dt) 
{
	position = initial_position;

	iPoint EnemyPos = { (int)initial_position.x + 32, (int)initial_position.y };
	iPoint PlayerPos{ (int)App->entity_m->player_entity->position.x + 30, (int)App->entity_m->player_entity->position.y + 46 };

	if ((abs(App->entity_m->player_entity->position.x - EnemyPos.x) < 400) && !move) 
	{
		c = 0;
		
		App->pathfinding->CreatePath(EnemyPos, PlayerPos);
		App->pathfinding->Ground(PlayerPos, path);

		move = true;
	}

	if (move) 
	{
		iPoint objective = { path[c].x,path[c].y };

		current_animation = &walk_left;

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
			current_animation = &walk_right;

			if (EnemyPos.x <= objective.x) 
			{
				c++;
				move = false;
			}
		}
	}
	else
	{
		current_animation = &walk_left;
	}

	if (abs(App->entity_m->player_entity->position.x - EnemyPos.x) >= 400) 
	{
		move = false;
	}
}

void j1Zombie_Enemy::Draw(float dt) 
{
	current_animation->speed = 10.f*dt;
	collider->SetPos(position.x, position.y);
	App->render->Blit(App->entity_m->GetEnemySprites(),position.x,position.y, &(current_animation->GetCurrentFrame()));
}