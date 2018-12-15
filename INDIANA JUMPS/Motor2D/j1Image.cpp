#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Image.h"

j1Image::j1Image(int x, int y, SDL_Rect rect, bool visible) :x(x), y(y), img(rect), visible(visible)
{
	graphics = App->tex->Load("gui/atlas_new.png");
}

bool j1Image::Update() 
{
	return true;
}

bool j1Image::PostUpdate() 
{
	if (visible == true)
	{
		App->render->Blit(graphics, x, y, &img);
	}
	
	return true;
}