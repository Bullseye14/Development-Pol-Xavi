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

public:
	SDL_Texture* graphics = nullptr;

	iPoint pos;
	float xspeed;
	float yspeed;

	SDL_Rect rect_player;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation run;
};
#endif
