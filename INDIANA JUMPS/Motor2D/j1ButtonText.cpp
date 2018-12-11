#include "j1ButtonText.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Fonts.h"


j1ButtonText::j1ButtonText(int x, int y, E_TYPE type, j1Module* mod, const char * text, _TTF_Font * font) : j1UI_Element(x, y, type, mod)
{
}

j1ButtonText::~j1ButtonText()
{
}

void j1ButtonText::Draw()
{
	//App->render->Blit(UI_tex...)
}
