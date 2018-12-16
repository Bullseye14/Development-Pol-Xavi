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
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Timer.h"
#include "SDL/include/SDL_timer.h"

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
	actualVolume = App->audio->CurrentVolume;

	run_pos_x = 300;
	run_pos_y = 148;
	
	App->scene->active = false;
	App->entity_m->active = false;

	IndianaJumps = App->tex->Load("textures/MainMenuSpritesheet.png");

	UI_spritesheet = App->tex->Load("gui/atlas_new.png");
	VolumeRect = { 0,225,500,75 };

	logo = { 224,0,304,169 };

	current_animation = &idle;

	//Background of the menu
	menuBackgroundTex = App->tex->Load("gui/MainMenu_TILED.png");
	BG_Rect = { 0, 0, 1024, 480 };

	//Spawn Buttons
	App->gui->element_list.add(App->gui->SpawnButton(550, 95, PLAY));
	App->gui->element_list.add(App->gui->SpawnButton(550, 235, CONTINUE));
	App->gui->element_list.add(App->gui->SpawnButton(800, 25, SETTINGS));
	App->gui->element_list.add(App->gui->SpawnButton(800, 165, CREDITS));
	App->gui->element_list.add(App->gui->SpawnButton(800, 305, EXIT));

	//Spawn Text
	App->gui->element_list.add(App->gui->SpawnText(615, 114, 0, TEXT, "PLAY", { 230,115,0,0 }));
	App->gui->element_list.add(App->gui->SpawnText(583, 255, 0, TEXT, "CONTINUE", { 230,115,0,0 }));
	App->gui->element_list.add(App->gui->SpawnText(848, 44, 0, TEXT, "SETTINGS", { 230,115,0,0 }));
	App->gui->element_list.add(App->gui->SpawnText(850, 184, 0, TEXT, "CREDITS", { 230,115,0,0 }));
	App->gui->element_list.add(App->gui->SpawnText(873, 325, 0, TEXT, "EXIT", { 230,115,0,0 }));

	//Spawn Image
	App->gui->element_list.add(App->gui->SpawnImage(30, 20, logo, true));

	return true;
}

bool j1MainMenu::Update(float dt)
{
	App->render->Blit(menuBackgroundTex, 0, 0, &BG_Rect);

	VolumeToMove = { 0,300,actualVolume,75 };

	if (manageVolume) { ManageVolume(); }

	ManageMenuAnimation();

	if (finishRun && comeFromScene == false)
	{
		run_pos_x += 12;
		App->render->Blit(IndianaJumps, run_pos_x, run_pos_y, &current_animation->GetCurrentFrame());
	}
	else if (mouseInButton == false) 
	{
		current_animation = &idle;
		App->render->Blit(IndianaJumps, run_pos_x, run_pos_y, &current_animation->GetCurrentFrame());
	}
	else 
	{
		current_animation = &running;
		App->render->Blit(IndianaJumps, run_pos_x, run_pos_y, &current_animation->GetCurrentFrame());
	}

	if (showVolume) 
	{
		App->gui->element_list.add(App->gui->SpawnButton(300, 400, VOLUME));
		App->render->Blit(UI_spritesheet, 300, 400, &VolumeRect);
		App->render->Blit(UI_spritesheet, 300, 400, &VolumeToMove);
		
		App->audio->MusicVolume(actualVolume / 4);
	}
	
	return true;
}

bool j1MainMenu::PostUpdate()
{
	if (ending) return false;
	else return true;
}

bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(IndianaJumps);
	App->tex->UnLoad(UI_spritesheet);
	App->tex->UnLoad(menuBackgroundTex);

	runAway = false;
	mouseInButton = false;
	mouseInVolume = false;
	finishRun = false;
	showVolume = false;
	ending = false;
	//App->tex->UnLoad();
	return true;
}

void j1MainMenu::ManageMenuAnimation()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	if (runAway) 
	{ 
		mouseInButton = true; 
		finishRun = true;
	}

	else if (
		((x > 550 && x < 748) && (y > 95 && y < 170 || y > 235 && y < 310))
		|| ((x > 800 && x < 999) && (y > 25 && y < 100 || y > 165 && y < 240 || y > 305 && y < 380))
			)
	{
		mouseInButton = true;
	}
	
	else 
	{
		mouseInButton = false;
	}
}

void j1MainMenu::ManageVolume() 
{
	int x, y;
	App->input->GetMousePosition(x, y);

	if (x > 300 && x < 800 && y > 400 && y < 475)
	{
		actualVolume = (x - 300);
	}
}

void j1MainMenu::GoToScene(int button)
{
	App->scene->current_level = nullptr;
	
	pressStart.ReadSec();

	// BUTTON 0 PLAY // BUTTON 1 CONTINUE //
	if (button == 0)
	{
		App->scene->LoadLevel(1);
	}

	if(button == 1)
	{
		App->LoadGame("save_game.xml");
		if (App->scene->choose_level == 1)
			App->scene->LoadLevel(1);
		else
			App->scene->LoadLevel(2);
	}

	App->fade->FadeToBlack(App->mainmenu, App->scene);
	number = button;

	runAway = true;

	if (clock.ReadSec() - pressStart.ReadSec() >= 3.0)
	{
		CleanUp();
		App->mainmenu->active = false;
	}

}
