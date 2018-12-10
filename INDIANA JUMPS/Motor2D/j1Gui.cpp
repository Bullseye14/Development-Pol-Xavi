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

SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}
// class Gui ---------------------------------------------------
