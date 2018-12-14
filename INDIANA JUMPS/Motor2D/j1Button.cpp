#include "j1App.h"
#include "j1Gui.h"
#include "j1Button.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "Animation.h"
#include "j1FadeToBlack.h"
#include "j1EntityManager.h"

j1Button::j1Button(BUTTON_TYPE type)
{
	buttonRect = { 0,0,199,75 };
	buttonOnHover = { 0,75,199,75 };
	buttonOnClick = { 0,150,199,75 };

	volumeRect = { 0,0,500,75 };

	b_type = type;
}

j1Button::~j1Button()
{
}

bool j1Button::Start() 
{
	if (b_type == VOLUME) 
	{
		App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &volumeRect);
	}
	else
		App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &buttonRect);
		
	
	return true;
}

bool j1Button::PostUpdate() 
{
	bool ret = true;
	
	switch (mouse_state) 
	{
	case MOUSE_NONE:
		if (b_type != VOLUME)
		{
			ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &buttonRect);
			break;
		}

	case MOUSE_HOVER:
		if (b_type != VOLUME)
		{
			ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &buttonOnHover);
			break;
		}

	case MOUSE_CLICK:
		if (b_type != VOLUME)
		{
			ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &buttonOnClick);
		}
		if (b_type == PLAY) 
		{
			App->mainmenu->GoToScene(0);
		}
		else if (b_type == CONTINUE) 
		{
			App->mainmenu->GoToScene(1);
		}
		else if (b_type == CREDITS)
		{
			ShellExecuteA(NULL, "open", "https://bullseye14.github.io/IndianaJumps/", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (b_type == EXIT) 
		{
			App->mainmenu->ending = true;
		}
		else if (b_type == SETTINGS) 
		{
			if (App->mainmenu->showVolume == false) 
			{
				App->mainmenu->showVolume = true;
			}
			
			else 
			{
				App->mainmenu->showVolume = false;
			}
		
		}
		else if (b_type == VOLUME)
		{
			App->mainmenu->manageVolume = true;
		}
		break;

	}
	
	DrawDebug();

	return ret;
}


bool j1Button::OnHover()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	bool ret = position.x + App->render->camera.x / (int)App->win->GetScale() < x 
		&& position.y + App->render->camera.y < y && position.x + App->render->camera.x / (int)App->win->GetScale() + buttonRect.w > x 
		&& position.y + buttonRect.h > y;

	return ret;
}

bool j1Button::OnClick()
{
	bool ret = false;

	if (OnHover())
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			ret = true;
		}
	}

	return ret;
}

void j1Button::DrawDebug()
{
	debugRect = { position.x, position.y, 20, 20 };
	if (debug == true) {
		App->render->DrawQuad(debugRect, 255, 0, 255, 50);
	}
}
