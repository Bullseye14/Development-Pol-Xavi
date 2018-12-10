#ifndef __j1BUTTON_H__
#define __j1BUTTON_H__

#include "j1UI_Element.h"

class SDL_Rect;
class SDL_Texture;

class j1Button : public j1UI_Element
{
public:

	j1Button(int x, int y, E_TYPE type, SDL_Rect* RecTex, const char* text, j1Module* mod, bool visible);
	~j1Button();

	void Draw();
	void Interaction();

private:

	SDL_Texture*	buttonTex = nullptr;
	SDL_Rect*		buttonRect = nullptr;
	bool			buttonOn = false;
};

#endif //__j1BUTTON_H__
