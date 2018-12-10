#ifndef __j1IMAGE_H__
#define __j1IMAGE_H__

#include "j1UI_Element.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"

class SDL_Rect;
class SDL_Texture;

class j1Image :public j1UI_Element
{
public:

	j1Image(int x, int y, E_TYPE type, SDL_Rect* rect, j1Module* mod);
	~j1Image();

	void Draw();

public:

	SDL_Rect*		imgRect = nullptr;
	SDL_Texture*	imgTex = nullptr;

};

#endif //__j1IMAGE_H__
