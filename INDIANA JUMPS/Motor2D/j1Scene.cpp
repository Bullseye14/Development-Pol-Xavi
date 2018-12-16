#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1Bird.h"
#include "j1Zombie.h"
#include "j1Gui.h"
#include "j1MainMenu.h"
#include "j1FadeToBlack.h"
#include "j1UI_Element.h"

#include "Brofiler/Brofiler.h"
#pragma comment ( lib, "Brofiler/ProfilerCore32.lib" )

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	// Creating a list with 2 the two maps
	map* map1 = new map(1, "desert_map_new.tmx");
	map* map2 = new map(2, "forest_map_new.tmx");

	mapList.add(map1);
	mapList.add(map2);

	current_level = mapList.start;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	PERF_START(ptimer);
	// Calling the function to load the map
	App->map->Load(mapList.start->data->map_name.GetString());
	path_img = App->tex->Load("path.png");
	
	// Creating the player
	App->entity_m->CreatePlayer();
	img = App->entity_m->player->graphics;

	//App->entity_m->AddEnemy(300, 200, BIRD);
	//App->entity_m->AddEnemy(1000, 325, ZOMBIE);
	//App->entity_m->AddEnemy(200, 300, COIN);

	//App->gui->element_list.add(App->gui->SpawnImage(800, 10, { 35, 375, 140, 50 }, true));

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::PaleTurquoise);
	
	// Load game
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
	{
		App->LoadGame("save_game.xml"); 
	}
	
	// Save game
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) 
	{
		App->SaveGame("save_game.xml"); 
	}

	CameraToPlayer();

	// Start from the beginning of the first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		// TODO
		LoadLevel(1);
	}

	// Start from the beginning of this level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->entity_m->player->Respawn();
	}

	App->map->Draw();
	//App->render->Blit(img, 0, 0);

	//HUD
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::PaleTurquoise);
	
	bool ret = true;

	
	

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{
		App->SaveGame("save_game.xml");
		App->fade->FadeToBlack(App->scene, App->mainmenu);
		App->mainmenu->active = true;
		/*App->entity_m->active = false;
		App->gui->active = true; 
		App->fade->free_gui = true;
		App->mainmenu->finishRun = false;
		App->mainmenu->run_pos_x = 300;*/
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	SuperClean();

	return true;
}

bool j1Scene::Load(pugi::xml_node& data) 
{
	current_level->data->level = data.child("level").attribute("num").as_int();
	
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const 
{
	pugi::xml_node level = data.append_child("level");
	level.append_attribute("num") = current_level->data->level;

	return true;
}

void j1Scene::LoadLevel(int number)
{
	// Loading the current level
	p2List_item<map*>* level = mapList.start;

	while (choose_level < number)
	{
		level = level->next;
		choose_level++;
	}
	current_level = level;

	if (current_level != nullptr)
	{
		//Clean up the level
		App->collision->CleanUp();
		App->map->CleanUp();

		if (current_level->data->level == 1) 
		{
			//Starting the level & player
			App->map->Load(current_level->data->map_name.GetString());
			App->collision->Start();
			App->entity_m->player->playerHitbox = nullptr;
			App->entity_m->player->Start();
		}
	}
}

void j1Scene::SuperClean() {

	App->tex->UnLoad(img);
	App->tex->UnLoad(path_img);
	//App->map->CleanUp();
	App->collision->CleanUp();
	/*App->tex->CleanUp();*/	// Don't know why but with tex->CleanUp() game crashes when click window exit
	App->gui->CleanUp();
}

void j1Scene::CameraToPlayer()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	if (App->entity_m->player->pos_player.x < 350) 
	{
		App->render->camera.x = 0;
	}
	else if (App->entity_m->player->pos_player.x > 5710)
	{
		App->render->camera.x = -5375;
	}
	else 
		App->render->camera.x = -App->entity_m->player->pos_player.x + w / 3;
	
}
void j1Scene::GoToMenu()
{
	load_gui = true;
	App->scene->CleanUp();
	App->mainmenu->Start();
}