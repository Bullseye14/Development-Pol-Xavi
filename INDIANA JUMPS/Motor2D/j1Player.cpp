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
	pos_player.x = config.child("pos").attribute("x").as_int();
	pos_player.y = config.child("pos").attribute("y").as_int();
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
	//changeJump = false;
	from_up = false;
	from_left = false;
	from_right = false;
	jumping = false;
	onfloor = false;

	playerHitbox = App->collision->AddCollider({ pos_player.x, pos_player.y, 32, 64 }, COLLIDER_PLAYER, this);

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

	

	//Check collisions


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && pos_player.x < 6400 - 64 && from_left == false) {
		
		if (current_animation == &idle) 
		{
			speed.x = 1;
			from_left = true;
			current_animation = &run;
		}
	}

	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && pos_player.x > 0 && from_right == false) {
		
		if (current_animation == &idle) 
		{
				speed.x = -1;
				from_right = true;
				current_animation = &run_left;
		}
	}

	else
	{
		speed.x = 0;
		if (current_animation != &jump) { current_animation = &idle; }
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && onfloor == true && doublejump > 0)
	{	
		onfloor = false;
		speed.y = -10;
		doublejump--;
	}
	if (onfloor == false) 
	{
		pos_player.y += speed.y;
	}
	else { speed.y = 0; }

	playerHitbox->SetPos(pos_player.x + 16, pos_player.y);

	Check_Collision(rect_player);

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
	App->render->Blit(graphics, pos_player.x, pos_player.y, &current_animation->GetCurrentFrame());

	/*if (!onfloor) 
	{
		speed.y += 1;
		if (speed.y > gravity) { speed.y = gravity; }
	}
	else speed.y = 0;*/
	
	pos_player.x += speed.x;
	pos_player.y += speed.y;


	return true;
}

bool j1Player::Check_Collision(const SDL_Rect &r) 
{
	return !(rect_player.y + rect_player.h < r.y ||
			rect_player.y > r.y + r.h ||
			rect_player.x + rect_player.w < r.x ||
			rect_player.x > r.x + r.w);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) 
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL)
	{
		//touches from above
		if ((c2->rect.y) > (c1->rect.y + c1->rect.h - 10)) { from_up = true; }
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10)) { from_right = true; }
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10)) { from_left = true; }
	}
	
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH)
	{
		//Die();
	}
}
