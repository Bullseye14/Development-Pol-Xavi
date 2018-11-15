#include "j1Zombie_Enemy.h"

j1Zombie_Enemy::j1Zombie_Enemy(int x, int y) : Entity(x,y) 
{
	App->entity_m->zombie_active = true;

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

	current_animation = &walk_left;

	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,32,64 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	initial_pos = initial_position.x;

}

bool j1Zombie_Enemy::Awake(pugi::xml_node& config) 
{
	config = App->LoadConfig(App->entity_m->config_file);

	config = config.child("entities").child("zombie");

	speed.x = config.child("speed").attribute("x").as_int();
	speed.y = config.child("speed").attribute("y").as_int();

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
	App->render->Blit(App->entity_m->GetEnemySprites(), position.x, position.y, &(current_animation->GetCurrentFrame()));
}