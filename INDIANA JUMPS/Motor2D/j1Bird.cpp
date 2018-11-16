#include "j1Bird.h"
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


j1Bird::j1Bird(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::BIRD)
{
	fly_right.PushBack({ 0,320,64,64 });
	fly_right.PushBack({ 64,320,64,64 });
	fly_right.PushBack({ 128,320,64,64 });
	fly_right.PushBack({ 192,320,64,64 });
	fly_right.loop = true;

	fly_left.PushBack({ 0,384,64,64 });
	fly_left.PushBack({ 64,384,64,64 });
	fly_left.PushBack({ 128,384,64,64 });
	fly_left.PushBack({ 192,384,64,64 });
	fly_left.loop = true;

	initialPosition.x = position.x = x;
	initialPosition.y = position.y = y;
}

j1Bird::~j1Bird()
{
}

bool j1Bird::Start()
{
	graphics = App->tex->Load("textures/Enemies_Spritesheet.png");

	animation = &fly_left;

	playerHitbox = App->collision->AddCollider({ (int)position.x, (int)position.y, 64, 64 }, COLLIDER_ENEMY, (j1Module*)App->entity_m);

	return true;
}

bool j1Bird::Update(float dt)
{
	playerHitbox->SetPos(position.x, position.y);

	return true;
}

bool j1Bird::CleanUp()
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr)
		playerHitbox->to_delete = true;

	return true;
}

void j1Bird::OnCollision(Collider * col_1, Collider * col_2)
{
}
