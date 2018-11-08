#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
enum MOVEMENT { STOPPED, MOVING };
enum DIRECTION_X { CENTER_X, LEFT, RIGHT };
enum DIRECTION_Y { CENTER_Y, UP, DOWN };

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
	void Respawn();
	void DoAnimations();
	//void Win();
	

	void OnCollision(Collider* c1, Collider* c2);
	void Check_Collision();


public:
	SDL_Texture* graphics = nullptr;

	MOVEMENT mov = STOPPED;
	DIRECTION_X dir_x = CENTER_X;
	DIRECTION_Y dir_y = CENTER_Y;

	fPoint start_pos;
	fPoint pos_player;
	fPoint pos_initial;
	iPoint speed;
	int playerheight = 64;
	int playerwidth = 32;
	int doublejump = 2;
	float gravity;
	float pos_jump;
	float pos_final;
	float max_speed_y;
	float jumpforce;

	bool GodMode = false;

	SDL_Rect rect_player;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation run;
	Animation run_left;
	Animation jump;
	Animation jump_left;

	Collider* playerHitbox;

	
	bool from_up;
	bool from_right;
	bool from_left;
	bool from_down;
	
	bool death;
	bool won;

	bool jumping;
	bool falling;
	bool onfloor;
	
};
#endif
