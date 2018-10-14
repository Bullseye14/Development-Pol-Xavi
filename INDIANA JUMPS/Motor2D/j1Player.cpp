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
#include <stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	current_animation = NULL;

	// Loading all animations

	idle.LoadAnimation("idle");
	run.LoadAnimation("run");
	run_left.LoadAnimation("run_left");
	jump.LoadAnimation("jump");
}
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	pos_player.x = config.child("pos").attribute("x").as_int();
	pos_player.y = config.child("pos").attribute("y").as_int();
	return true;
}
bool j1Player::Start()
{
	// Spritesheet
	if (graphics == nullptr) 
	{
		graphics = App->tex->Load("textures/Spritesheet.png");
	}

	// Initial values
	current_animation = &idle;
	speed.x = 0;
	speed.y = 0;
	from_up = false;
	from_left = false;
	from_right = false;
	jumping = false;
	onfloor = false;
	death = false;
	won = false;

	// Player hitbox
	playerHitbox = App->collision->AddCollider({ (int)pos_player.x, (int)pos_player.y, 32, 64 }, COLLIDER_PLAYER, this);

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
bool j1Player::Update(float dt)
{
	from_up = false;
	from_left = false;
	from_right = false;
	from_down = false;

	DoAnimations();

	
	//MOVEMENT OF THE PLAYER
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && pos_player.x < 6400 - 64 && from_left == false) 
	{
		mov = MOVING;
		dir_x = RIGHT;				// Player moving to the right
		speed.x = 5;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && pos_player.x > 0 && from_right == false) 
	{
		mov = MOVING;
		dir_x = LEFT;				// Player moving to the left
		speed.x = -5;
	}

	else
	{
		mov = STOPPED;				// Player stopped
		speed.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && falling == false && doublejump > 0)
	{	
		App->audio->PlayFx(App->audio->jump);

		jumping = true;
		onfloor = false;
		doublejump--;
		pos_final = pos_player.y - playerheight * 2;
		
		gravity = 6.0f;				// Player jumping

		// TO FINISH (player is appearing from above, not moving upwards)

		while (gravity > 0)
		{
			current_animation = &jump;
			speed.y -= gravity;
			pos_player.y += speed.y;
			gravity -= 1.0f;
		}

		if (gravity <= 0) 
		{
			/*from_up == false;
			falling = true;*/
			speed.y = 0.0f;
		}
	}
	
	// Updating the hitbox
	playerHitbox->SetPos(pos_player.x + 16, pos_player.y);

	if (falling == true && current_animation != &jump)
	{
		current_animation = &jump;
	}

	// God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		GodMode = !GodMode;
		if (GodMode == true) {
			playerHitbox->type = COLLIDER_GOD;
		}
		else if (GodMode == false) {
			playerHitbox->type = COLLIDER_PLAYER;
		}
	}
	
	// Start from the beginning of the first level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		// TODO
		Respawn();
	}

	// Start from the beginning of this level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{
		Respawn();
	}

	return true;
}
bool j1Player::PostUpdate()
{
	
	App->render->Blit(graphics, pos_player.x, pos_player.y, &current_animation->GetCurrentFrame());

	Check_Collision();
	
	pos_player.x += speed.x;
	pos_player.y += speed.y;
	
	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) 
{
	// PLAYER & WALL
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL)
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
		// touches from bottom
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 10)) 
		{
			from_down = true;
		}
	}
	
	// PLAYER && DEATH
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH)
	{
		death = true;
	}
	
	// PLAYER && END
	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_END)
	{
		Win();
	}

	// GOD && WALL
	if (c1->type == COLLIDER_GOD && c2->type == COLLIDER_WALL)
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

	// GOD && DEATH
	if (c1->type == COLLIDER_GOD && c2->type == COLLIDER_DEATH) {
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
	if (!from_up)
	{
		pos_player.y += 5.0f;			// Falling
	}
	else if (from_up)
	{
		speed.y = 0;
		onfloor = true;
		current_animation = &idle;		// Touching floor
		doublejump = 2;
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
}
void j1Player::Respawn() 
{
	App->fade->FadeToBlack(App->scene, App->scene, 1.0f);

	pos_player = start_pos;
}

void j1Player::Win() 
{
	won = true;
	Respawn();
}

void j1Player::DoAnimations() 
{

	if (onfloor == true) 
	{
		switch (dir_x)
		{
		case LEFT:
			current_animation = &run_left;
			break;
		case RIGHT:
			current_animation = &run;
			break;
		default:
			break;
		}
	}
	else current_animation = &jump;

	if (mov == STOPPED)
	{
		current_animation = &idle;

		if (onfloor == false) { current_animation = &jump; }
	}
}
