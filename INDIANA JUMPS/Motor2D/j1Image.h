#ifndef __j1IMAGE_H__
#define __j1IMAGE_H__

#include "j1UI_Element.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"

class j1Image : public j1UI_Element 
{
public:

	j1Image(int x, int y, SDL_Rect rect, bool visible = false);
	~j1Image() {};

	bool Update();
	bool PostUpdate();

	bool visible = false;

private:

	SDL_Rect		img;
	SDL_Texture*	graphics = nullptr;
	int				x, y;
};

#endif //__j1IMAGE_H__
