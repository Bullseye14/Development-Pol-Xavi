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
#include "j1Button.h"
#include "j1MainMenu.h"
#include "j1ButtonText.h"

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
	atlas = App->tex->Load("gui/atlas.png");

	p2List_item<j1UI_Element*>* element_iterator = element_list.start;

	while (element_iterator != nullptr)
	{
		element_iterator->data->Start();
		element_iterator = element_iterator->next;
	}

	return true;
}

bool j1Gui::PreUpdate(float dt)
{
	p2List_item<j1UI_Element*>* element_iterator;
	 element_iterator = element_list.start;

	while (element_iterator != nullptr) 
	{
		element_iterator->data->PreUpdate(dt);
		element_iterator = element_iterator->next;
	}

	return true;
}

bool j1Gui::Update(float dt) 
{
	p2List_item<j1UI_Element*>* element_iterator;
	element_iterator = element_list.start;

	while (element_iterator != nullptr)
	{
		element_iterator->data->Update(dt);
		element_iterator = element_iterator->next;
	}

	return true;
}

bool j1Gui::PostUpdate()
{
	p2List_item<j1UI_Element*>* element_iterator;
	element_iterator = element_list.start;

	while (element_iterator != nullptr)
	{
		element_iterator->data->PostUpdate();
		element_iterator = element_iterator->next;
	}

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

j1UI_Element*j1Gui::SpawnButton(int x, int y, BUTTON_TYPE type, const char* text)
{
	j1UI_Element* button = nullptr;
	button = new j1Button(text, type);
	
	button->position.x = x;
	button->position.y = y;

	return button;
}




