#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	//void ChangeDirection();
	void OnCollision(Collider* c1, Collider* c2);
	//void Check_Collision();


public:
	SDL_Texture* graphics = nullptr;

	iPoint pos;
	iPoint speed;
	int playerheight;
	int playerwidth;

	int yspeed;

	SDL_Rect rect_player;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation run;
	Animation jump;

	Collider* playerHitbox;

	bool changeJump;
	bool from_up;
	bool from_right;
	bool from_left;
	bool from_down;
	bool jumping;
	bool falling;
	bool contact;
};
#endif
