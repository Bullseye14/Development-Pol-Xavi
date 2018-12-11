#include "j1UI_Element.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"

j1UI_Element::j1UI_Element(int x, int y, E_TYPE type, j1Module* mod) : position(x, y), type(type), callback(mod)
{
}

j1UI_Element::~j1UI_Element()
{
}