#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1App.h"
#include "j1Module.h"
#include "Animation.h"
#include "j1Textures.h"

struct SDL_Texture;

class j1MainMenu :public j1Module
{
public:

	j1MainMenu();
	virtual ~j1MainMenu();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	
	void ManageMenuAnimation();
	void GoToScene();
	
	bool ending = false;

private:

	SDL_Texture *	UI_spritesheet = nullptr;

	SDL_Texture *	IndianaJumps = nullptr;
	SDL_Rect		PlayerRect;

	SDL_Texture *	menuBackgroundTex = nullptr;
	SDL_Rect		BG_Rect;
	
	Animation*		current_animation;
	Animation		idle, running;

	bool mouseInButton = false;

};

#endif // __J1MAINMENU_H__
