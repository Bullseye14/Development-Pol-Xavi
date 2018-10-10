#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1GamePhysics.h"
#include <stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	//MUST CALCULATE THE RECT
	idle.PushBack({0, 0, 64, 64});
	idle.PushBack({64, 0, 64, 64});
	/*idle.PushBack({ 0, 64, 64, 64 });
	idle.PushBack({ 64, 64, 64, 64 });
	idle.PushBack({0, 128, 64, 64 });
	idle.PushBack({ 64, 128, 64, 64 });
	idle.PushBack({ 0, 192, 64, 64 });
	idle.PushBack({ 64, 192, 64, 64 });
	idle.PushBack({0, 256, 64, 64 });
	idle.PushBack({ 64, 256, 64, 64 });*/



	/*run.PushBack({310, 499, 346, 475});
	run.PushBack({656, 499, 363, 492});
	run.PushBack({1020, 499, 367, 485});
	run.PushBack({1395, 500, 402, 462});
	run.PushBack({1799, 499, 401, 469});
	run.PushBack({2199, 504, 342, 467});
	run.PushBack({2540, 505, 350, 488});
	run.PushBack({0, 993, 364, 473});
	run.PushBack({365, 993, 358, 449});
	run.PushBack({723, 993, 371, 464});*/

}
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	pos.x = config.child("pos").attribute("x").as_int();
	pos.y = config.child("pos").attribute("y").as_int();
	return true;
}
bool j1Player::Start()
{
	if (graphics == nullptr) {
		graphics = App->tex->Load("textures/Idle.png");
	}

	current_animation = &idle;
	speed.x = 0;
	speed.y = 0;
	yspeed = 7;
	changeJump = false;
	from_up = false;
	from_left = false;
	from_right = false;
	jumping = false;
	contact = false;

	current_animation = &idle;

	playerHitbox = App->collision->AddCollider({ pos.x, pos.y, 64, 64 }, COLLIDER_PLAYER, this);

	return true;
}
bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics); 
	return true;
}
bool j1Player::Update(float dt)
{
	

	from_up = false;
	from_left = false;
	from_right = false;
	from_down = false;


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		speed.x = 4;
		if (current_animation == &idle) 
		{
			current_animation = &run;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		speed.x = -4;
		if (current_animation == &idle) 
		{
			current_animation = &run;
		}
	}

	else
	{
		speed.x = 0;
		if (current_animation != &jump) { current_animation = &idle; }
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && falling == false && jumping == false)
	{
		current_animation = &jump;
		speed.y = -25;

	}

	//ChangeDirection();

	playerHitbox->setPos(pos.x, pos.y);

	if (speed.y > 0) //falling
	{
		falling = true;
		jumping = false;
	}
	if (speed.y < 0) //jumping
	{
		jumping = true;
		falling = false;
	}

	if (falling == true && current_animation != &jump)
	{
		current_animation = &jump;
	}

	return true;
}
bool j1Player::PostUpdate()
{
	if (changeJump == false)
		App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	else
		App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

	//Check_Collision();
	
	pos.x += speed.x;
	pos.y += speed.y;


	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) 
{

}
