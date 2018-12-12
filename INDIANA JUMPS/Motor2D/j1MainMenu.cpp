#include "j1MainMenu.h"
#include "p2List.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1UI_Element.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Fonts.h"
#include "j1Button.h"
#include "j1Window.h"

j1MainMenu::j1MainMenu()
{
	name.create("mainmenu");

	idle.PushBack({ 0,0,156,244 });
	idle.PushBack({ 162,0,155,244 });
	idle.PushBack({ 1,245,157,244 });
	idle.PushBack({ 163,245,155,244 });
	idle.PushBack({ 3,490,155,244 });
	idle.PushBack({ 164,491,156,244 });
	idle.PushBack({ 3,735,155,244 });
	idle.PushBack({ 162,734,156,244 });
	idle.PushBack({ 1,978,157,244 });
	idle.PushBack({ 161,978,156,244 });
	idle.speed = 0.4f;

	running.PushBack({ 326,10,202,239 });
	running.PushBack({ 530,8,203,247 });
	running.PushBack({ 326,260,195,245 });
	running.PushBack({ 530,262,202,232 });
	running.PushBack({ 326,520,202,236 });
	running.PushBack({ 530,520,204,236 });
	running.PushBack({ 326,772,195,246 });
	running.PushBack({ 530,768,198,238 });
	running.PushBack({ 326,1025,195,225 });
	running.PushBack({ 530,1030,204,233 });
	running.speed = 0.3f;

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

	IndianaJumps = App->tex->Load("textures/MainMenuSpritesheet.png");

	current_animation = &idle;

	//Background of the menu
	menuBackgroundTex = App->tex->Load("gui/MainMenu_TILED.png");
	BG_Rect = { 0, 0, 1024, 480 };

	App->gui->element_list.add(App->gui->SpawnButton(800, 25, PLAY));
	App->gui->element_list.add(App->gui->SpawnButton(800, 95, CONTINUE));
	App->gui->element_list.add(App->gui->SpawnButton(800, 165, SETTINGS));
	App->gui->element_list.add(App->gui->SpawnButton(800, 235, CREDITS));
	App->gui->element_list.add(App->gui->SpawnButton(800, 305, EXIT));


	//Loading Fonts
	//App->font->font_title = App->font->Load("fonts/Adventure.ttf", 12);
	//App->font->font_buttons = App->font->Load("fonts/West.ttf", 8);

	return true;
}

bool j1MainMenu::Update(float dt)
{
	App->render->Blit(menuBackgroundTex, 0, 0, &BG_Rect);

	ManageMenuAnimation();

	if (mouseInButton == false) {
		current_animation = &idle;
		App->render->Blit(IndianaJumps, 200, 148, &current_animation->GetCurrentFrame());
	}
	else {
		current_animation = &running;
		App->render->Blit(IndianaJumps, 200, 148, &current_animation->GetCurrentFrame());
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



void j1MainMenu::ManageMenuAnimation()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	if ((x > 800 && x < 933) && 
		(y > 25 && y < 75 
		|| y > 95 && y < 145
		|| y > 165 && y < 215
		|| y > 235 && y < 285
		|| y > 305 && y < 355))
	{
		mouseInButton = true;
	}
	
	else 
	{
		mouseInButton = false;
	}
	
}

void j1MainMenu::GoToScene()
{
	App->mainmenu->active = false;
	App->gui->active = false; //temporal
	App->scene->active = true;
	App->entity_m->active = true;
}
