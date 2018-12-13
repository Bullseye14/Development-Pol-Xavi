#ifndef __j1BUTTON_H__
#define __j1BUTTON_H__

#include "j1Gui.h"
#include "j1UI_Element.h"
#include "SDL/include/SDL.h"
#include "p2Defs.h"

enum BUTTON_TYPE 
{
	NO_BUTTON,
	PLAY,
	CONTINUE,
	EXIT,
	SETTINGS,
	CREDITS,
	VOLUME
};

class j1Button : public j1UI_Element
{

public:

	j1Button(BUTTON_TYPE type);
	~j1Button();

	bool Start();
	bool PostUpdate();

	bool OnHover();
	bool OnClick();

private:

	BUTTON_TYPE		b_type;
	SDL_Rect		buttonRect, buttonOnHover, buttonOnClick;
	SDL_Rect		volumeRect;
	p2SString		buttonText;
};

#endif //__j1BUTTON_H__
