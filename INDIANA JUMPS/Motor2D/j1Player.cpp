#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include <stdio.h>

j1Player::j1Player(int x, int y) : Entity(x, y)
{
//	name.create("player");
	App->entity_m->player_active = true;
	current_animation_player = current_animation = NULL;
	graphics = NULL;

	// Loading all animations
	idle.LoadAnimation("idle");
	run.LoadAnimation("run");
	run_left.LoadAnimation("run_left");
	jump.LoadAnimation("jump");
	jump_left.LoadAnimation("jump_left");
	slide_l.LoadAnimation("slide_l");
	slide_r.LoadAnimation("slide_r");

	initial_pos = initial_position.x;
}

j1Player::~j1Player()
{
	if (collider != nullptr) 
	{
		collider->to_delete = true;
	}
}

bool j1Player::Start()
{
	collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 32, 64 }, COLLIDER_PLAYER, (j1Module*)App->entity_m);
	
	// Spritesheet
	spritesheet = App->tex->Load("textures/Spritesheet.png");

	return true;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	pugi::xml_document	config_file;
	pugi::xml_node		config2;

	config2 = App->LoadConfig(config_file);

	config2 = config2.child("enitities").child("player");

	position.x = config2.child("pos").attribute("x").as_int();
	position.y = config2.child("pos").attribute("y").as_int();
	speed.x = config2.child("speed").attribute("x").as_int();
	speed.y = config2.child("speed").attribute("y").as_int();
	from_up = config2.child("fromup").attribute("value").as_bool();
	from_down = config2.child("fromdown").attribute("value").as_bool();
	from_left = config2.child("fromleft").attribute("value").as_bool();
	from_right = config2.child("fromright").attribute("value").as_bool();
	jumping = config2.child("jumping").attribute("value").as_bool();
	onfloor = config2.child("onfloor").attribute("value").as_bool();
	max_speed_y = config2.child("max_speed_y").attribute("value").as_float();
	jumpforce = config2.child("jumpforce").attribute("value").as_float();
	death = config2.child("death").attribute("value").as_bool();
	won = config2.child("won").attribute("value").as_bool();
	start_freefalling = config2.child("start_freefalling").attribute("value").as_bool();
	sliding = config2.child("sliding").attribute("value").as_bool();
	max_speed_x = config2.child("max_speed_x").attribute("value").as_float();
	slidingforce = config2.child("slidingforce").attribute("value").as_float();
	speed_slide = config2.child("speed_slide").attribute("value").as_float();

	return true;
}

void j1Player::MoveEntity(float dt)
{
	dt = dt / 1000;

	idle.speed = 10.f*dt;
	jump.speed = 10.f*dt;
	jump_left.speed = 10.f*dt;
	run.speed = 10.f*dt;
	run_left.speed = 10.f*dt;
	slide_l.speed = 10.f*dt;
	slide_r.speed = 10.f*dt;

	//if (start_freefalling == false)
	{
		//MOVEMENT OF THE PLAYER

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && pos_player.x < 6400 - 64 && from_left == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
			{
				mov = MOVING;
				dir_x = SLIDE_R;
				speed.x = slidingforce;
				App->audio->PlayFx(App->audio->slide);
				sliding = true;
			}
			else
			{
				sliding = false;
				mov = MOVING;
				dir_x = RIGHT;				// Player moving to the right
				speed.x = 5 * dt;
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
			}
			else
			{
				sliding = false;
				mov = MOVING;
				dir_x = LEFT;				// Player moving to the left
				speed.x = -5 * dt;
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
			Jump(dt);

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
	//collider->SetPos(pos_player.x + 16, pos_player.y);

	if (falling == true && current_animation != &jump)
	{
		if (dir_x == RIGHT)
			current_animation = &jump;
		else
			current_animation = &jump_left;
	}

	// God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		GodMode = !GodMode;
		if (GodMode == true)
		{
			collider->type = COLLIDER_GOD;
		}
		else if (GodMode == false)
		{
			collider->type = COLLIDER_PLAYER;
		}
	}

	if (jumping == true)
	{
		current_animation = &jump;
		speed.y -= 2.0f * dt;
		if (speed.y <= max_speed_y)
		{
			jumping = false;
		}
	}
	else if (!from_up)
	{
		current_animation = &jump;
		if (GodMode == false)
			speed.y = 7.0f * dt;			// Falling
		else
			speed.y = 2.5f * dt;
	}
	else if (from_up)
	{
		speed.y = 0;
		onfloor = true;
		current_animation = &idle;		// Touching floor
		doublejump = 2 * dt;
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
		//		Respawn();
	}

	if (won == true)
	{
		if (App->scene->current_level->data->level == 1)
		{
			App->scene->LoadLevel(2); // Switching between levels when winning
		}
		else if (App->scene->current_level->data->level == 2)
			App->scene->LoadLevel(1);


		if (sliding == true)
		{
			collider->type = COLLIDER_SLIDE;
		}
		else if (GodMode == true)
		{
			collider->type = COLLIDER_GOD;
		}
		else
		{
			collider->type = COLLIDER_PLAYER;
		}
	}

	position.x += speed.x;
	position.y += speed.y;
}

void j1Player::Jump(float dt) 
{
	if (jumping == true)
	{
		current_animation = &jump;
		speed.y -= 2.0f;
		if (speed.y <= max_speed_y)
		{
			jumping = false;
		}
	}
	else if (!from_up)
	{
		current_animation = &jump;
		if (GodMode == false)
			speed.y = 7.0f;			// Falling
		else
			speed.y = 2.5f;
	}
	else if (from_up)
	{
		speed.y = 0;
		onfloor = true;
		current_animation = &idle;		// Touching floor
		doublejump = 2;
		start_freefalling = false;
	}
}

void j1Player::Draw(float dt) 
{
	collider->SetPos(position.x, position.y);
	App->render->Blit(spritesheet, position.x, position.y, &(current_animation->GetCurrentFrame()));
}

//bool j1Player::CleanUp()
//{
//	// Unloading everything
//	App->tex->UnLoad(graphics); 
//	if (playerHitbox != nullptr)
//	{
//		playerHitbox->to_delete = true;
//		playerHitbox = nullptr;
//	}
//	return true;
//}
//
//bool j1Player::Load(pugi::xml_node& data) 
//{
//	// Player position
//	pos_player.x = data.child("position").attribute("x").as_int();
//	pos_player.y = data.child("position").attribute("y").as_int();
//
//	// God Mode
//	GodMode = data.child("god_mode").attribute("active").as_bool();
//
//	return true;
//}
//
//bool j1Player::Save(pugi::xml_node& data) const 
//{
//	pugi::xml_node position = data.append_child("position");
//
//	position.append_attribute("x") = pos_player.x;
//	position.append_attribute("y") = pos_player.y;
//
//	pugi::xml_node god_mode = data.append_child("god_mode");
//	god_mode.append_attribute("active") = GodMode;
//
//	return true;
//}
//
//bool j1Player::Update(float dt)
//{
//
//	from_up = false;
//	from_left = false;
//	from_right = false;
//	from_down = false;
//
//	// Animations from xml
//	DoAnimations();
//	
//	if (start_freefalling == false)
//	{
//		//MOVEMENT OF THE PLAYER
//		
//		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && pos_player.x < 6400 - 64 && from_left == false)
//		{
//			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
//			{
//				// TODO : Timer
//				mov = MOVING;
//				dir_x = SLIDE_R;
//				speed.x = slidingforce;
//				App->audio->PlayFx(App->audio->slide);
//				sliding = true;
//			}
//			else 
//			{
//				sliding = false;
//				mov = MOVING;
//				dir_x = RIGHT;				// Player moving to the right
//				speed.x = 5;
//			}
//		}
//		
//		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && pos_player.x > 0 && from_right == false)
//		{
//			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
//			{
//				mov = MOVING;
//				dir_x = SLIDE_L;
//				speed.x = -slidingforce;
//				App->audio->PlayFx(App->audio->slide);
//				sliding = true;
//			}
//			else
//			{
//				sliding = false;
//				mov = MOVING;
//				dir_x = LEFT;				// Player moving to the left
//				speed.x = -5;
//			}
//		}
//
//		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && onfloor == true && from_left == false)
//		{
//			mov = MOVING;
//			dir_x = SLIDE_R;
//			speed.x = slidingforce;
//			App->audio->PlayFx(App->audio->slide, 1);
//			sliding = true;
//		}
//
//		else
//		{
//			mov = STOPPED;				// Player stopped
//			speed.x = 0;
//		}
//
//		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && falling == false && doublejump > 0)
//		{
//			if (GodMode == false) 
//			{
//				doublejump--;				// Double jump
//				App->audio->PlayFx(App->audio->jump);
//				jumping = true;
//				onfloor = false;
//				speed.y = -jumpforce;
//			}
//			else if (GodMode == true) 
//			{
//				App->audio->PlayFx(App->audio->jump);
//				jumping = true;
//				onfloor = false;
//				speed.y = -jumpforce;
//			}
//		}
//	}
//	
//	// Updating the hitbox
//	playerHitbox->SetPos(pos_player.x + 16, pos_player.y);
//
//	if (falling == true && current_animation != &jump)
//	{
//		if (dir_x == RIGHT)
//			current_animation = &jump;
//		else
//			current_animation = &jump_left;
//	}
//
//	// God mode
//	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) 
//	{
//		GodMode = !GodMode;
//		if (GodMode == true) 
//		{
//			playerHitbox->type = COLLIDER_GOD;
//		}
//		else if (GodMode == false) 
//		{
//			playerHitbox->type = COLLIDER_PLAYER;
//		}
//	}
//
//
//	if (sliding == true) 
//	{
//		playerHitbox->type = COLLIDER_SLIDE;
//	}
//	else if (GodMode == true) 
//	{
//		playerHitbox->type = COLLIDER_GOD;
//	}
//	else
//	{
//		playerHitbox->type = COLLIDER_PLAYER;
//	}
//
//	return true;
//}
//
//bool j1Player::PostUpdate()
//{
//	// Drawing the player
//	App->render->Blit(graphics, pos_player.x, pos_player.y, &current_animation->GetCurrentFrame());
//	
//	// Checking collisions
//	Check_Collision();
//	
//	// Moving the player
//	Move();
//	
//	return true;
//}
//
//void j1Player::OnCollision(Collider* c1, Collider* c2) 
//{
//	// PLAYER & WALL
//	if (c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_SLIDE || c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_WALL)
//	{
//		// touches from above
//		if ((c2->rect.y) > (c1->rect.y + c1->rect.h - 10)) 
//		{ 
//			from_up = true; 
//		}
//		// touches from right
//		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10)) 
//		{ 
//			from_right = true; 
//		}
//		// touches from left
//		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10)) 
//		{ 
//			from_left = true; 
//		}
//		// touches from bottom
//		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 10)) 
//		{
//			from_down = true;
//		}
//	}
//	
//	// PLAYER && DEATH
//	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH || c2->type == COLLIDER_ENEMY)
//	{
//		death = true;
//	}
//
//	// PLAYER && END
//	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD || c1->type==COLLIDER_SLIDE) && c2->type == COLLIDER_END)
//	{
//		won = true;
//		start_freefalling = true;
//		current_animation = &jump;
//	}
//	
//	if (c1->type == COLLIDER_SLIDE && c2->type == COLLIDER_ENEMY) 
//	{
//		death = false;
//		c2->to_delete = true;
//		// Still not created:
//		// App->enemy->Dies();
//	}
//
//	// SLIDE && DEATH
//	if (c1->type == COLLIDER_SLIDE && c2->type == COLLIDER_DEATH)
//	{
//		death = true;
//	}
//
//	// GOD && WALL or DEATH (is the same, GOD can walk through death)
//	if (c1->type == COLLIDER_GOD && c2->type == COLLIDER_WALL || c2->type == COLLIDER_DEATH)
//	{
//		if ((c2->rect.y) > (c1->rect.y + c1->rect.h - 10))
//		{
//			from_up = true;
//		}
//		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10))
//		{
//			from_right = true;
//		}
//		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10))
//		{
//			from_left = true;
//		}
//	}
//}
//
//void j1Player::Check_Collision() 
//{
	//if (jumping == true)
	//{
	//	current_animation = &jump;
	//	speed.y -= 2.0f;
	//	if (speed.y <= max_speed_y) 
	//	{
	//		jumping = false;
	//	}
	//}
	//else if (!from_up)
	//{
	//	current_animation = &jump;
	//	if (GodMode == false)
	//		speed.y = 7.0f;			// Falling
	//	else
	//		speed.y = 2.5f;
	//}
	//else if (from_up)
	//{
	//	speed.y = 0;
	//	onfloor = true;
	//	current_animation = &idle;		// Touching floor
	//	doublejump = 2;
	//	start_freefalling = false;
	//}
	//
	//if (from_left)						// Colliding from left
	//{
	//	if (speed.x < 0) { speed.x = 0; }
	//}
	//
	//if (from_right)						// Colliding from right
	//{
	//	if (speed.x > 0) { speed.x = 0; }
	//}
	//
	//if (death == true)					// Player dies
	//{
	//	death = false;
	//	Respawn();
	//}

	//if (won == true) 
	//{
	//	if (App->scene->current_level->data->level == 1) 
	//	{
	//		App->scene->LoadLevel(2); // Switching between levels when winning
	//	}
	//	else if(App->scene->current_level->data->level == 2)
	//		App->scene->LoadLevel(1);
//	}
//}
//
//void j1Player::Respawn() 
//{ 
//	if (App->scene->current_level->data->level == 1) 
//	{
//		start_pos.y = 340;
//	}
//	else start_pos.y = 250;
//	pos_player = start_pos;		// Player respawns
//}
//
//void j1Player::DoAnimations() 
//{
//	// If the player is moving
//	if (mov == MOVING)
//	{
//		//  is touching the ground
//		if (onfloor == true)
//		{
//			switch (dir_x)
//			{
//			case LEFT:
//				current_animation = &run_left;
//				break;
//			case RIGHT:
//				current_animation = &run;
//				break;
//			case SLIDE_L:
//				current_animation = &slide_l;
//				break;
//			case SLIDE_R:
//				current_animation = &slide_r;
//				break;
//			default:
//				break;
//			}
//		}
//		// is jumping
//		else if (onfloor == false)
//		{
//			switch (dir_x) 
//			{
//			case LEFT:
//				current_animation = &jump_left;
//				break;
//			case RIGHT:
//				current_animation = &jump;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	// If he is not moving
//	if (mov == STOPPED)
//	{
//		// is on the floot
//		current_animation = &idle;
//
//		// is jumping
//		if (onfloor == false) { 
//			switch (dir_x) {
//			case LEFT:
//				current_animation = &jump_left;
//				break;
//			case RIGHT:
//				current_animation = &jump;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}
//
//void j1Player::Move() 
//{
	//pos_player.x += speed.x;
	//pos_player.y += speed.y;
//}
