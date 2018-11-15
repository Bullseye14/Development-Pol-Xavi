#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Entity.h"

struct SDL_Texture;
enum MOVEMENT { STOPPED, MOVING };
enum DIRECTION_X { CENTER_X, LEFT, RIGHT, SLIDE_L, SLIDE_R };
enum DIRECTION_Y { CENTER_Y, UP, DOWN };

class j1Player : public Entity
{
public:
	j1Player(int x, int y);
	~j1Player();
	bool Awake(pugi::xml_node& config);
	bool Start();
	void MoveEntity(float dt);
	void Jump(float dt);
	void Draw(float dt);
//	bool Update(float dt);
//	bool PostUpdate();
//	bool CleanUp();
//	void Respawn();
//	void DoAnimations();
//	void Move();
//	void Win();

	void SetPos(float x, float y) 
	{
		position.x = x;
		position.y = y;
	}

	float getX() { return position.x; }
	float getY() { return position.y; }

	Animation* current_animation_player = nullptr;
	Animation idle;
	Animation run;
	Animation run_left;
	Animation jump;
	Animation jump_left;
	Animation slide_l;
	Animation slide_r;
	
//	 Save and Load
//	bool Load(pugi::xml_node&);
//	bool Save(pugi::xml_node&) const;

//	void OnCollision(Collider* c1, Collider* c2);
//	void Check_Collision();


public:

	MOVEMENT mov = STOPPED;
	DIRECTION_X dir_x = CENTER_X;
	DIRECTION_Y dir_y = CENTER_Y;

	fPoint start_pos;
	fPoint pos_player;
	fPoint pos_initial;
	iPoint speed;
	int		playerheight = 64;
	int		playerwidth = 32;
	int		doublejump = 2;
	float	gravity;
	float	pos_jump;
	float	pos_final;
	float	max_speed_y;
	float	jumpforce;
	float	max_speed_x;
	float	slidingforce;
	float	speed_slide;
	float	pos_slide;
//	int		past_ticks;

	bool GodMode = false;

	SDL_Rect rect_player;

private:

	SDL_Texture* graphics = nullptr;

public:

	bool from_up;
	bool from_right;
	bool from_left;
	bool from_down;
	
	bool death;
	bool won;

	bool jumping;
	bool falling;
	bool onfloor;
	bool sliding;
	bool start_freefalling;
	
};
#endif
