#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include "j1Gui.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

enum E_TYPE 
{
	NO_TYPE,
	IMAGE,
	BUTTON,
	LABEL,
	TEXT
};

enum MOUSE_ACTION 
{
	NO_ACTION,
	HOVER,
	CLICK
};

class j1UI_Element 
{
public:

	j1UI_Element(int x, int y, E_TYPE type);
	~j1UI_Element();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void Draw();
	void DetectMovement(E_TYPE type);


public:

	E_TYPE type;
	MOUSE_ACTION mouse_action;
	int x, y;
	SDL_Texture* UI_Texture = nullptr;

	bool OnHover;
	bool OnClick;

};

#endif // __UI_ELEMENT_H__
