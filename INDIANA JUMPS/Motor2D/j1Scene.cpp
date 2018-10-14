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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

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
	App->map->Load(mapList.start->data->map_name.GetString());
	
	//App->map->Load("desert_map_new.tmx");
	//App->map->Load("forest_map_new.tmx");
	img = App->player->graphics;
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
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();		
		
	App->map->Draw();
	App->render->Blit(img, 0, 0);

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadLevel(int number)
{
	p2List_item<map*>* level = mapList.start;
	
	for (int i = 1; i < number; i++)
	{
		level = level->next;
	}
	current_level = level;

	if (current_level != nullptr)
	{
		//Clean up the level
		App->collision->CleanUp();
		App->map->CleanUp();

		////Starting the level & player
		App->map->Load(current_level->data->map_name.GetString());
		App->player->playerHitbox = nullptr;
		App->player->Start();
	}
	else
	{
		LOG("Current level is nullptr", number);
	}
}
