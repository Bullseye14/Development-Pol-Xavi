#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1App.h"
#include "j1Module.h"
#include "Animation.h"
#include "j1Textures.h"

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

	SDL_Texture * menuBackgroundTex = nullptr;
	SDL_Rect BG_Rect;

};

#endif // __J1MAINMENU_H__
