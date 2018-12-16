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
#include "j1FadeToBlack.h"
#include "j1MainMenu.h"
#include <stdio.h>

#include "Brofiler/Brofiler.h"
#pragma comment ( lib, "Brofiler/ProfilerCore32.lib" )

j1Player::j1Player(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::PLAYER)
{
	//name.create("player");

	animation = NULL;

	// Loading all animations
	idle.LoadAnimation("idle");
	run.LoadAnimation("run");
	run_left.LoadAnimation("run_left");
	jump.LoadAnimation("jump");
	jump_left.LoadAnimation("jump_left");
	slide_l.LoadAnimation("slide_l");
	slide_r.LoadAnimation("slide_r");
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	pugi::xml_document config_file;
	pugi::xml_node node;
	
	node = App->LoadConfig(config_file);
	node = node.child("entity").child("player");

	// Reading initial values from xml
	pos_player.x = node.child("pos").attribute("x").as_int();
	pos_player.y = node.child("pos").attribute("y").as_int();
	speed.x = node.child("speed").attribute("x").as_int();
	speed.y = node.child("speed").attribute("y").as_int();
	from_up = node.child("fromup").attribute("value").as_bool();
	from_down = node.child("fromdown").attribute("value").as_bool();
	from_left = node.child("fromleft").attribute("value").as_bool();
	from_right = node.child("fromright").attribute("value").as_bool();
	jumping = node.child("jumping").attribute("value").as_bool();
	onfloor = node.child("onfloor").attribute("value").as_bool();
	max_speed_y = node.child("max_speed_y").attribute("value").as_float();
	jumpforce = node.child("jumpforce").attribute("value").as_float();
	death = node.child("death").attribute("value").as_bool();
	won = node.child("won").attribute("value").as_bool();
	start_freefalling = node.child("start_freefalling").attribute("value").as_bool();
	sliding = node.child("sliding").attribute("value").as_bool();
	max_speed_x = node.child("max_speed_x").attribute("value").as_float();
	slidingforce = node.child("slidingforce").attribute("value").as_float();
	speed_slide = node.child("speed_slide").attribute("value").as_float();

	//pos_initial = pos_player;

	return true;
}
bool j1Player::Start()
{
	// Spritesheet
	if (graphics == nullptr) 
	{
		graphics = App->tex->Load("textures/Spritesheet.png");
	}

	start_freefalling = true;
	won = false;

	// When loading a new level, initial postition
	pos_player.x = pos_initial.x;
	pos_player.y = pos_initial.y;

	// Initial values
	animation = &jump;
	
	if (start_freefalling == true) 
	{
		speed.x = 0;
		animation = &jump;
	}
		
	// Player hitbox
	playerHitbox = App->collision->AddCollider({ (int)pos_player.x, (int)pos_player.y, 32, 64 }, COLLIDER_PLAYER, App->entity_m);

	return true;
}

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Red);
	
	from_up = false;
	from_left = false;
	from_right = false;

	// Animations from xml
	DoAnimations();

	if (start_freefalling == false)
	{
		//MOVEMENT OF THE PLAYER

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && pos_player.x < 6400 - 64 && from_left == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
			{
				// TODO : Timer
				mov = MOVING;
				dir_x = SLIDE_R;
				speed.x = slidingforce;
				App->audio->PlayFx(App->audio->slide);
				sliding = true;
				animation = &slide_r;
			}
			else
			{
				sliding = false;
				mov = MOVING;
				dir_x = P_RIGHT;				// Player moving to the right
				speed.x = 5;
				animation = &run;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && pos_player.x > 0 && from_right == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
			{
				mov = MOVING;
				dir_x = SLIDE_L;
				speed.x = -slidingforce;
				App->audio->PlayFx(App->audio->slide);
				sliding = true;
				animation = &slide_l;
			}
			else
			{
				sliding = false;
				mov = MOVING;
				dir_x = P_LEFT;				// Player moving to the left
				speed.x = -5;
				animation = &run_left;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
		{
			mov = MOVING;
			dir_x = SLIDE_R;
			speed.x = slidingforce;
			App->audio->PlayFx(App->audio->slide, 1);
			sliding = true;
		}

		else
		{
			mov = STOPPED;				// Player stopped
			speed.x = 0;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && falling == false && doublejump > 0)
		{
			if (GodMode == false)
			{
				doublejump--;				// Double jump
				App->audio->PlayFx(App->audio->jump);
				jumping = true;
				onfloor = false;
				speed.y = -jumpforce;
			}
			else if (GodMode == true)
			{
				App->audio->PlayFx(App->audio->jump);
				jumping = true;
				onfloor = false;
				speed.y = -jumpforce;
			}
		}
	}

	// Updating the hitbox
	playerHitbox->SetPos(pos_player.x + 16, pos_player.y);

	if (falling == true && animation != &jump)
	{
		if (dir_x == P_RIGHT)
			animation = &jump;
		else
			animation = &jump_left;
	}

	// God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		GodMode = !GodMode;
		if (GodMode == true)
		{
			playerHitbox->type = COLLIDER_GOD;
		}
		else if (GodMode == false)
		{
			playerHitbox->type = COLLIDER_PLAYER;
		}
	}


	if (sliding == true)
	{
		playerHitbox->type = COLLIDER_SLIDE;
	}
	else if (GodMode == true)
	{
		playerHitbox->type = COLLIDER_GOD;
	}
	else
	{
		playerHitbox->type = COLLIDER_PLAYER;
	}

	return true;
}


bool j1Player::PostUpdate()
{
	BROFILER_CATEGORY("Player PostUpdate", Profiler::Color::DarkRed);
	
	// Drawing the player
	App->render->Blit(graphics, pos_player.x, pos_player.y, &animation->GetCurrentFrame());

	// Checking collisions
	Check_Collision();

	// Moving the player
	Move();

	return true;
}


bool j1Player::CleanUp()
{
	// Unloading everything
	App->tex->UnLoad(graphics); 
	if (playerHitbox != nullptr)
	{
		playerHitbox->to_delete = true;
		playerHitbox = nullptr;
	}
	return true;
}

bool j1Player::Load(pugi::xml_node& data) 
{
	// Player position
	pos_player.x = data.child("position").attribute("x").as_int();
	pos_player.y = data.child("position").attribute("y").as_int();

	// God Mode
	GodMode = data.child("god_mode").attribute("active").as_bool();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const 
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = pos_player.x;
	position.append_attribute("y") = pos_player.y;

	pugi::xml_node god_mode = data.append_child("god_mode");
	god_mode.append_attribute("active") = GodMode;

	return true;
}

void j1Player::Move()
{
	pos_player.x += speed.x;
	pos_player.y += speed.y;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) 
{
	// PLAYER & WALL
	if (c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_SLIDE || c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_WALL)
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
	
	// PLAYER && DEATH
	if (c1->type == COLLIDER_PLAYER && (c2->type == COLLIDER_DEATH || c2->type == COLLIDER_ENEMY))
	{
		death = true;
	}

	// PLAYER && END
	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD || c1->type==COLLIDER_SLIDE) && c2->type == COLLIDER_END)
	{
		won = true;
		start_freefalling = true;
		animation = &jump;
	}
	
	if (c1->type == COLLIDER_SLIDE && c2->type == COLLIDER_ENEMY) 
	{
		death = false;
		c2->to_delete = true;
		// Still not created:
		// App->enemy->Dies();
	}

	// SLIDE && DEATH
	if (c1->type == COLLIDER_SLIDE && c2->type == COLLIDER_DEATH)
	{
		death = true;
	}

	// GOD && WALL or DEATH (is the same, GOD can walk through death)
	if (c1->type == COLLIDER_GOD && c2->type == COLLIDER_WALL || c2->type == COLLIDER_DEATH)
	{
		if ((c2->rect.y) > (c1->rect.y + c1->rect.h - 10))
		{
			from_up = true;
		}
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10))
		{
			from_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10))
		{
			from_left = true;
		}
	}
}

void j1Player::Check_Collision() 
{
	if (jumping == true)
	{
		animation = &jump;
		speed.y -= 2.0f;
		if (speed.y <= max_speed_y) 
		{
			jumping = false;
		}
	}
	else if (!from_up)
	{
		animation = &jump;
		if (GodMode == false)
			speed.y = 7.0f;			// Falling
		else
			speed.y = 2.5f;
	}
	else if (from_up)
	{
		speed.y = 0;
		onfloor = true;
//		animation = &idle;		// Touching floor
		doublejump = 2;
		start_freefalling = false;
	}
	
	if (from_left)						// Colliding from left
	{
		if (speed.x < 0) { speed.x = 0; }
	}
	
	if (from_right)						// Colliding from right
	{
		if (speed.x > 0) { speed.x = 0; }
	}
	
	if (death == true)					// Player dies
	{
		death = false;
		Respawn();
	}

	if (won == true) 
	{
		if (App->scene->current_level->data->level == 1)
		{
			App->scene->LoadLevel(2); // Switching between levels when winning
		}
		
		else if (App->scene->current_level->data->level == 2)
		{
			App->scene->GoToMenu();
		}
	}
}

void j1Player::Respawn() 
{ 
	if (App->scene->current_level->data->level == 1) 
	{
		start_pos.y = 340;
	}
	else start_pos.y = 250;
	pos_player = start_pos;		// Player respawns
}

void j1Player::DoAnimations() 
{
	// If the player is moving
	if (mov == MOVING)
	{
		//  is touching the ground
		if (onfloor == true)
		{
			switch (dir_x)
			{
			case P_LEFT:
				animation = &run_left;
				break;
			case P_RIGHT:
				animation = &run;
				break;
			case SLIDE_L:
				animation = &slide_l;
				break;
			case SLIDE_R:
				animation = &slide_r;
				break;
			default:
				break;
			}
		}
		// is jumping
		else if (onfloor == false)
		{
			switch (dir_x) 
			{
			case P_LEFT:
				animation = &jump_left;
				break;
			case P_RIGHT:
				animation = &jump;
				break;
			default:
				break;
			}
		}
	}
	// If he is not moving
	if (mov == STOPPED)
	{
		// is on the floot
		animation = &idle;

		// is jumping
		if (onfloor == false) { 
			switch (dir_x) {
			case P_LEFT:
				animation = &jump_left;
				break;
			case P_RIGHT:
				animation = &jump;
				break;
			default:
				break;
			}
		}
	}
}