#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Image.h"


j1Image::j1Image(int x, int y, E_TYPE type, SDL_Rect* rect, j1Module* mod) : j1UI_Element(x, y, type, mod)
{
	imgRect = rect;
}


j1Image::~j1Image()
{
}

void j1Image::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x - App->render->camera.x, position.y - App->render->camera.y, imgRect);
}
