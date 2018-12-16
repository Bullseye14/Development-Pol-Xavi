#include "Globals.h"
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1EntityManager.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"


j1FadeToBlack::j1FadeToBlack()
{
	screen = { 0, 0, 1024 * SCREEN_SIZE, 480 * SCREEN_SIZE };
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	bool ret = true;

	if (current_step == fade_step::none)
	{
		ret == true;
	}
	else
	{
		switch (current_step)
		{
		case fade_step::fade_to_black:
		{
			if (transp <= 250)
			{
				transp += 5;
			}

			if (free_gui)
			{
				free_gui = false;
				App->gui->startgame = true;
			}
			if (clock.ReadSec() >= 2.0)
			{
				App->render->camera.x = 0;
				App->render->camera.y = 0;
				App->entity_m->active = true;


				if (to_enable == App->scene)
				{
					if (App->mainmenu->number == 0) 
					{
						App->mainmenu->GoToScene(0); 
					}
					else 
					{
						App->mainmenu->GoToScene(1);
						App->LoadGame("save_game.xml");
					}
				}
				else
				{
					App->scene->GoToMenu();
				}

				to_enable->active = true;
				current_step = fade_step::fade_from_black;
			}
		}
		break;

		case fade_step::fade_from_black:
		{
			if (transp >= 6)
			{
				transp -= 6;
			}
			else
			{
				current_step = fade_step::none;
			}
		}
		break;
		}

		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, transp);
		SDL_RenderFillRect(App->render->renderer, &screen);
	}

	return ret;
}


bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on)
{
	bool ret = false;
	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		transp = 0;
		free_gui = true;
		to_disable = module_off;
		to_enable = module_on;
		clock.Start();
		ret = true;
	}
	return ret;
}
