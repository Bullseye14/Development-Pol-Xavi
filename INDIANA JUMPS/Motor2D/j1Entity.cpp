#include "j1Module.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"

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

void j1Entity::CheckCollision()
{
	if (App->entity_m->player->jumping == true)
	{
		App->entity_m->player->current_animation = &App->entity_m->player->jump;
		App->entity_m->player->speed.y -= 2.0f;
		if (App->entity_m->player->speed.y <= App->entity_m->player->max_speed_y)
		{
			App->entity_m->player->jumping = false;
		}
	}
	else if (!App->entity_m->player->from_up)
	{
		App->entity_m->player->current_animation = &App->entity_m->player->jump;
		if (App->entity_m->player->GodMode == false)
			App->entity_m->player->speed.y = 7.0f;			// Falling
		else
			App->entity_m->player->speed.y = 2.5f;
	}
	else if (App->entity_m->player->from_up)
	{
		App->entity_m->player->speed.y = 0;
		App->entity_m->player->onfloor = true;
		App->entity_m->player->current_animation = &App->entity_m->player->idle;		// Touching floor
		App->entity_m->player->doublejump = 2;
		App->entity_m->player->start_freefalling = false;
	}

	if (App->entity_m->player->from_left)						// Colliding from left
	{
		if (App->entity_m->player->speed.x < 0) { App->entity_m->player->speed.x = 0; }
	}

	if (App->entity_m->player->from_right)						// Colliding from right
	{
		if (App->entity_m->player->speed.x > 0) { App->entity_m->player->speed.x = 0; }
	}

	if (App->entity_m->player->death == true)					// Player dies
	{
		App->entity_m->player->death = false;
		App->entity_m->player->Respawn();
	}

	if (App->entity_m->player->won == true)
	{
		if (App->scene->current_level->data->level == 1)
		{
			App->scene->LoadLevel(2); // Switching between levels when winning
		}
		else if (App->scene->current_level->data->level == 2)
			App->scene->LoadLevel(1);
	}
}
