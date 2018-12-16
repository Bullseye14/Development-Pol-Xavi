#ifndef __j1UI_ELEMENT_H__
#define __j1UI_ELEMENT_H__

#include "p2Point.h"
#include "j1Module.h"
#include "SDL\include\SDL.h"

enum FONT_TYPE
{
	TITLE_FONT,
	BUTTON_FONT
};

enum E_TYPE
{
	NONE_UI,
	BACKGROUND,
	BUTTON,
	TEXT,
	IMAGE
};

enum UIEvents
{
	MOUSE_NONE,
	MOUSE_HOVER,
	MOUSE_CLICK,
	MOUSE_RELEASE
};

class j1UI_Element
{
public:
	j1UI_Element() {};
	~j1UI_Element() {};

	virtual bool Start() { return true; }
	bool PreUpdate(float dt) { return true; }
	virtual bool Update(float dt);
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp();

	virtual bool OnClick() { return true; };
	virtual bool OnHover() { return true; };

	//virtual void SelectFont(FONT_TYPE font);
	
public:
	UIEvents		mouse_state = MOUSE_NONE;

	SDL_Rect		debugRect;
	bool			debug = false;

	SDL_Texture*	UI_tex = nullptr;
	SDL_Rect		UI_rect;
	iPoint			position;
	E_TYPE			type;
	//FONT_TYPE		font_type;
	
	bool hover = false;
	bool none = false;
	bool click = false;

	bool fix = false;
};

#endif // __j1UI_ELEMENT_H__ 