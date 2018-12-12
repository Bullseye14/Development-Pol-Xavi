#include "j1ButtonText.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Fonts.h"

j1ButtonText::j1ButtonText(int x, int y, TEXT_TYPE type, uint font_num, const char * text, SDL_Color color)
{
	font = font_num;
	string = text;
	text_color = color;
}

j1ButtonText::j1ButtonText()
{
}
