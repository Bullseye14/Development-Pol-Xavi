#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;

class Collider;
class Player;

struct map 
{
	map(int number, char* name) 
	{
		this->map_name.create(name);
		level = number;
	}
	
	p2SString map_name;
	int level;
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	void LoadLevel(int num);

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SuperClean();

	p2List<map*> mapList;
	p2List_item<map*> *current_level;

	int choose_level = 1;

	int limitcamera_left;
	int limitcamera_right;
	int limitplayer;

private:
	SDL_Texture * img;
};

#endif // __j1SCENE_H__