#include "j1Zombie.h"
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

#include "Brofiler/Brofiler.h"
#pragma comment ( lib, "Brofiler/ProfilerCore32.lib" )

j1Zombie::j1Zombie(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::ZOMBIE)
{
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
	walk_left.speed = 0.1f;

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
	walk_right.speed = 0.1f;

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
	die_left.speed = 0.1f;

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
	die_right.speed = 0.1f;

	initialPosition.x = position.x = x;
	initialPosition.y = position.y = y;
}

j1Zombie::~j1Zombie()
{
}

bool j1Zombie::Awake(pugi::xml_node& config)
{
	pugi::xml_node bird = config.child("bird");

	// variables here above

	return true;
}
bool j1Zombie::Start()
{
	graphics = App->tex->Load("textures/Enemies_Spritesheet.png");

	animation = &walk_right;

	playerHitbox = App->collision->AddCollider({ (int)position.x, (int)position.y, 40, 40 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	speed.x = 40;
	speed.y = 40;

	return true;
}

bool j1Zombie::Update(float dt)
{
	BROFILER_CATEGORY("Zombie Update", Profiler::Color::DarkGreen);
	
	playerHitbox->SetPos(position.x + 10, position.y + 13);
	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

	return true;
}

bool j1Zombie::CleanUp()
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr)
		playerHitbox->to_delete = true;

	return true;
}

void j1Zombie::OnCollision(Collider * col_1, Collider * col_2)
{
	if (col_1->type == COLLIDER_SLIDE) {
		if (col_2->type == COLLIDER_ENEMY) {
			App->tex->UnLoad(graphics);
		}
	}
}
