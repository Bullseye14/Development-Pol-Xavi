#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include<stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	//MUST CALCULATE THE RECT
	idle.PushBack({0, 0, 310, 484});
	idle.PushBack({310, 0, 310, 484});
	idle.PushBack({620, 0, 310,484});
	idle.PushBack({930, 0, 310, 484});
	idle.PushBack({1240, 3, 310, 484});
	idle.PushBack({1550, 3, 310, 484});
	idle.PushBack({ 1860, 0, 310, 484 });
	idle.PushBack({2170, 3, 310, 484});
	idle.PushBack({2480, 3, 310, 484});
	idle.PushBack({0, 488, 310, 484});


	run.PushBack({310, 499, 346, 475});
	run.PushBack({656, 499, 363, 492});
	run.PushBack({1020, 499, 367, 485});
	run.PushBack({1395, 500, 402, 462});
	run.PushBack({1799, 499, 401, 469});
	run.PushBack({2199, 504, 342, 467});
	run.PushBack({2540, 505, 350, 488});
	run.PushBack({0, 993, 364, 473});
	run.PushBack({365, 993, 358, 449});
	run.PushBack({723, 993, 371, 464});

}
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	return true;
}
bool j1Player::Start()
{
	if (graphics == nullptr) {
		graphics = App->tex->Load("textures/spriteindiana.png");
	}

	current_animation = &idle;
	xspeed = 1.5;
	yspeed = 1.5;
	pos.x = 5;
	pos.y = 10;

	return true;
}
bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics); 
	return true;
}
bool j1Player::Update(float dt)
{
	// Here we declare the GetKet(SCANCODE) for moving the player
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos.x += xspeed;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

	}

	rect_player.x = pos.x;
	rect_player.y = pos.y;

	App->render->DrawQuad(rect_player, 0, 0, 50, 50);

	return true;
}
bool j1Player::PostUpdate()
{
	// Here we call the Blit function
	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame());


	return true;
}