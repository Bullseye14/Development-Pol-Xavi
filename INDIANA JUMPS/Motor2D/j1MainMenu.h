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
	
	void GoToScene();

private:

	SDL_Texture * UI_spritesheet = nullptr;

	SDL_Texture * menuBackgroundTex = nullptr;
	SDL_Rect BG_Rect;

	//j1UI_Element* B_play = nullptr;
	//SDL_Rect R_play;



};

#endif // __J1MAINMENU_H__
