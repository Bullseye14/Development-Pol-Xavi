#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1UI_Element.h"
#include "j1MenuBackground.h"
#include "j1Button.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

j1Gui::~j1Gui()
{}

bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;
	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	return ret;
}

bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	//menuBackgroundTex = App->tex->Load("gui/");

	return true;
}

bool j1Gui::PreUpdate()
{
	return true;
}
bool j1Gui::PostUpdate()
{
	return true;
}

bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	return true;
}

SDL_Texture* j1Gui::GetMenuBackground() const
{
	return menuBackgroundTex;
}

SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
void j1Gui::SpawnMenuBackground(int x, int y, E_TYPE type, j1Module * mod)
{
	j1UI_Element* elem = new j1MenuBackground(x, y, type, mod);
	element_list.add(elem);
}

j1UI_Element* j1Gui::SpawnButton(int x, int y, E_TYPE type, SDL_Rect* rect, j1Module* mod, const char * text, bool visible)
{
	j1UI_Element* elem = new j1Button(x, y, type, rect, mod, text, visible);
	element_list.add(elem);

	return elem;
}