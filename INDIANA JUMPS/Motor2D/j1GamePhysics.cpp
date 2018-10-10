#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1GamePhysics.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1Map.h"

j1GamePhysics::j1GamePhysics() 
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;

}

j1GamePhysics::~j1GamePhysics() { }

bool j1GamePhysics::PreUpdate() 
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

bool j1GamePhysics::Update(float dt) 
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr) { continue; }
			

		c1 = colliders[i];
		c1->Update();

		for (uint h = i + 1; h < MAX_COLLIDERS; ++h)
		{
			if (colliders[h] == nullptr)
				continue;

			c2 = colliders[h];
			c2->Update();

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
				{
					c1->callback->OnCollision(c1, c2);
				}

				if (matrix[c2->type][c1->type] && c2->callback)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}

		}
	}

	DebugDraw();

	return true;
}

void j1GamePhysics::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_WALL:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_DEATH:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		}
	}
}

bool j1GamePhysics::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1GamePhysics::AddCollider(SDL_Rect rect, COLLIDER_TYPE coltype, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, coltype, callback);
			break;
		}
	}

	return ret;
}

bool j1GamePhysics::EraseCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

bool Collider::Update()
{
	return true;
}