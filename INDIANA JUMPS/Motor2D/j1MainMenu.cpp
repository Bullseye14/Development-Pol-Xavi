#include "j1MainMenu.h"
#include "p2List.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1UI_Element.h"
#include "j1Input.h"
#include "j1EntityManager.h"

j1MainMenu::j1MainMenu()
{
	name.create("mainmenu");
}

j1MainMenu::~j1MainMenu()
{
}

bool j1MainMenu::Awake(pugi::xml_node &)
{
	return true;
}

bool j1MainMenu::Start()
{
	App->scene->active = false;
	App->entity_m->active = false;

	//Background of the menu
	menuBackgroundTex = App->tex->Load("gui/MainMenu.png");
	BG_Rect = { 0, 0, 1024, 480 };

	App->gui->element_list.add(App->gui->SpawnButton(445, 200, PLAY, "PLAY"));
	App->gui->element_list.add(App->gui->SpawnButton(445, 300, CONTINUE, "CONTINUE"));
	App->gui->element_list.add(App->gui->SpawnButton(445, 400, EXIT, "EXIT"));

	return true;
}

bool j1MainMenu::Update(float dt)
{
	App->render->Blit(menuBackgroundTex, 0, 0, &BG_Rect);

	//App->render->Blit(playTex, 0, 0, &playRect);

	//p2List_item<j1Button*>* button_iterator;
	//button_iterator = App->gui->element_list.start;

	//while (button_iterator != nullptr) {
	//	button_iterator->data->Draw();
	//}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) 
	{
		GoToScene();
	}
	return true;
}

bool j1MainMenu::PostUpdate()
{
	return true;
}

bool j1MainMenu::CleanUp()
{
	return true;
}

void j1MainMenu::GoToScene()
{
	App->mainmenu->active = false;
	App->scene->active = true;
	App->entity_m->active = true;

}
