#ifndef __j1UI_ELEMENT_H__
#define __j1UI_ELEMENT_H__
#include "j1Gui.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h" 

enum E_TYPE
{
	NONE,
	BACKGROUND,
	BUTTON,
	TEXT,
	IMAGE
};

class j1UI_Element : public j1Gui
{
public:
	j1UI_Element(int x, int y, E_TYPE type, j1Module* mod);
	~j1UI_Element();
	virtual void Draw() {};
	
public:
	SDL_Texture*	UI_tex = nullptr;
	SDL_Rect		UI_rect;
	iPoint			position;
	E_TYPE			type;
	j1Module*		callback = nullptr;
	
};
#endif// __UIELEMENTS_H__ 