#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Image.h"

j1Image::j1Image(SDL_Rect rect, bool visible)
{
	img = rect;
	isVisible = visible;
	graphics = App->tex->Load("gui/atlas_new.png");
}


bool j1Image::PostUpdate() 
{
	if (isVisible == true)
	{
		App->render->Blit(graphics, position.x, position.y, &img);
	}
	
	return true;
}