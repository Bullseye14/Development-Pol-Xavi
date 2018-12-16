#include "j1Gui.h"
#include "j1UI_Element.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Gui.h"

bool j1UI_Element::Update(float dt) 
{
	switch (mouse_state) 
	{
	case UIEvents::MOUSE_NONE:
		if (OnHover()) 
		{ 
			hover = true;
			none = false;
			click = false;
			mouse_state = UIEvents::MOUSE_HOVER;
			break;
		}
	case UIEvents::MOUSE_HOVER:
		if (!OnHover())
		{
			none = true;
			hover = false;
			click = false;
			mouse_state = UIEvents::MOUSE_NONE;
		}
		if (OnClick())
		{
			click = true;
			none = false;
			hover = false;
			mouse_state = UIEvents::MOUSE_CLICK;
		}
		break;

	case UIEvents::MOUSE_CLICK:
		if (!OnClick())
		{
			hover = true;
			none = false;
			click = false;
			mouse_state = UIEvents::MOUSE_HOVER;
		}
		break;

	default:
		break;
	}

	/*if (fix == true)
	{
		position = {}
	}*/


	return true;
}
bool j1UI_Element::CleanUp()
{
	App->tex->UnLoad(App->gui->GetAtlas());
	App->tex->UnLoad(App->gui->menuBackgroundTex);
	return true;
}

