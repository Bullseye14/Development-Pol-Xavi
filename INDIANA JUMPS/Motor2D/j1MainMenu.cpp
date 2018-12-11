#include "j1MainMenu.h"
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

	menuBackgroundTex = App->tex->Load("gui/atlas.png");
	BG_Rect = { 0, 0, 700, 500 };

	/*playTex = App->tex->Load("gui/b_1");
	playRect = { 0, 0, 500, 500 };*/

	//App->gui->SpawnMenuBackground(0, 0, BACKGROUND, this);
	
	return true;
}

bool j1MainMenu::Update(float dt)
{
	App->render->Blit(menuBackgroundTex, 0, 0, &BG_Rect);

	//App->render->Blit(playTex, 0, 0, &playRect);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
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
