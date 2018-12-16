#include "j1ButtonText.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Fonts.h"

j1ButtonText::j1ButtonText(uint fontnum, E_TYPE type, const char * text, SDL_Color color)
{
	font = fontnum;
	string = text;
	text_color = color;
	string = text;
}

j1ButtonText::j1ButtonText()
{
}

bool j1ButtonText::Start()
{
	SelectFont();

	return true;
}

bool j1ButtonText::PostUpdate()
{
	App->render->Blit(text_texture, position.x, position.y);

	return true;
}

void j1ButtonText::SelectFont()
{
	if (font == 0) {
		text_texture = App->font->Print(string, text_color, App->font->Load("fonts/CenturyGothic.ttf", 26));
	}
	else if (font == 1){
		text_texture = App->font->Print(string, text_color, App->font->Load("fonts/burnstown dam.ttf", 82));
	}
}




