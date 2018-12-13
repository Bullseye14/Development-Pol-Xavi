#include "j1Player.h"
#include "j1Coin.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Module.h"

j1Coin::j1Coin(int x, int y, ENTITY_TYPES type) : j1Entity(x, y, ENTITY_TYPES::COIN) 
{
	idle.PushBack({ 199,0,25,25 });
	idle.loop = false;
}

j1Coin::~j1Coin(){}

bool j1Coin::Start() 
{
	graphics = App->tex->Load("gui/atlas_new.png");
	animation = &idle;

	playerHitbox = App->collision->AddCollider({ (int)position.x, (int)position.y, 25, 25 } , COLLIDER_COIN, (j1Module*)App->entity_m);
	
	speed = { 0,0 };

	return true;
}

bool j1Coin::Update(float dt) 
{
	playerHitbox->SetPos(position.x, position.y);

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

	if (taken) 
	{
		CleanUp();
	}

	return true;
}

bool j1Coin::CleanUp() 
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr) { playerHitbox->to_delete = true; }

	return true;
}

void j1Coin::OnCollision(Collider* c1, Collider* c2) 
{
	if ((c1->type == COLLIDER_PLAYER || c1->type == COLLIDER_SLIDE || c1->type == COLLIDER_GOD) && c2->type == COLLIDER_COIN) 
	{
		taken = true;
	}
}