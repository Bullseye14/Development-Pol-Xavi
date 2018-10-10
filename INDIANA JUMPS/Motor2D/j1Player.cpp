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

	run.PushBack({ 128, 0, 64, 64 });
	run.PushBack({ 192, 0, 64, 64 });
	run.PushBack({ 128, 64, 64, 64 });
	run.PushBack({ 192, 64, 64, 64 });
	run.PushBack({ 128, 128, 64, 64 });
	run.PushBack({ 192, 128, 64, 64 });
	run.PushBack({ 128, 192, 64, 64 });
	run.PushBack({ 192, 192, 64, 64 });
	run.PushBack({ 128, 256, 64, 64 });
	run.PushBack({ 192, 256, 64, 64 });
	run.speed = 0.1f;
	
	run_left.PushBack({ 320, 0, 64, 64 });
	run_left.PushBack({ 256, 0, 64, 64 });
	run_left.PushBack({ 320, 64, 64, 64 });
	run_left.PushBack({ 256, 64, 64, 64 });
	run_left.PushBack({ 320, 128, 64, 64 });
	run_left.PushBack({ 256, 128, 64, 64 });
	run_left.PushBack({ 320, 192, 64, 64 });
	run_left.PushBack({ 256, 192, 64, 64 });
	run_left.PushBack({ 320, 256, 64, 64 });
	run_left.PushBack({ 256, 256, 64, 64 });
	run_left.speed = 0.1f;

	jump.PushBack({ 384, 0, 64, 64 });
	jump.PushBack({ 448, 0, 64, 64 });
	jump.PushBack({ 384, 64, 64, 64 });
	jump.PushBack({ 448, 64, 64, 64 });
	jump.PushBack({ 384, 128, 64, 64 });
	jump.PushBack({ 448, 128, 64, 64 });
	jump.PushBack({ 384, 192, 64, 64 });
	jump.PushBack({ 448, 192, 64, 64 });
	jump.PushBack({ 384, 256, 64, 64 });
	jump.PushBack({ 448, 256, 64, 64 });
	jump.speed = 0.1f;
	jump.loop = false;
	
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
		graphics = App->tex->Load("textures/Spritesheet.png");
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
		speed.x = 2;
		if (current_animation == &idle) 
		{
			current_animation = &run;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		speed.x = -2;
		if (current_animation == &idle) 
		{
			current_animation = &run_left;
		}
	}

	else
	{
		speed.x = 0;
		if (current_animation != &jump) { current_animation = &idle; }
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && falling == false && jumping == false)
	{	
		
		for (int i = -2; i = 2; i++)
		{
			speed.y = i;
			current_animation = &jump;
		}
		
		//speed.y = 0;

	}

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
	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame());

	//Check_Collision();
	
	pos.x += speed.x;
	pos.y += speed.y;


	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) 
{

}
