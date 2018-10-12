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

	void OnCollision(Collider* c1, Collider* c2);
	void Check_Collision();


public:
	SDL_Texture* graphics = nullptr;

	fPoint pos_player;
	iPoint speed;
	int playerheight = 64;
	int playerwidth = 32;
	int doublejump = 2;
	float gravity = 0.0005f;
	float pos_jump;
	float pos_initial;
	float pos_final;

	SDL_Rect rect_player;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation run;
	Animation run_left;
	Animation jump;
	Animation jump_left;

	Collider* playerHitbox;

	//bool changeJump;

	// TODO SIDES
	// Different type of collisions if they come from different sides
	bool from_up;
	bool from_right;
	bool from_left;
	bool from_down;
	bool isdeath;
	// --------------------------------------------------------------

	bool jumping;
	bool falling;
	bool onfloor;
};
#endif
