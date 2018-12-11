#include "j1App.h"
#include "j1MenuBackground.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Window.h"


j1MenuBackground::j1MenuBackground(int x, int y, E_TYPE type, j1Module* mod) : j1UI_Element(x, y, type, mod)
{
}

j1MenuBackground::~j1MenuBackground()
{
}

void j1MenuBackground::Draw()
{
	//App->render->Blit(App->gui->GetAtlas(), position.x, position.y, {});
}
