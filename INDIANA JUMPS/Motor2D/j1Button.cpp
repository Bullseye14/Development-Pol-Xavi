#include "j1App.h"
#include "j1Gui.h"
#include "j1Button.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"

j1Button::j1Button(int x, int y, E_TYPE type, SDL_Rect* rect, j1Module* mod, bool visible) : j1UI_Element(x, y, type, mod)
{
	
}


j1Button::~j1Button()
{

}


void j1Button::Draw()
{
	//App->render->Blit(UI_tex, position.x, position.y, NULL);
}

void j1Button::Interaction()
{
	
}