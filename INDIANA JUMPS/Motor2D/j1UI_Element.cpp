#include "j1Gui.h"
#include "j1UI_Element.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Map.h"

bool j1UI_Element::Update(float dt) 
{
	switch (mouse_state) 
	{
	case UIEvents::MOUSE_NONE:
		if (OnHover()) 
		{ 
			mouse_state = UIEvents::MOUSE_HOVER;
			break;
		}
	case UIEvents::MOUSE_HOVER:
		if (!OnHover())
		{
			mouse_state = UIEvents::MOUSE_NONE;
		}
		if (OnClick())
		{
			mouse_state = UIEvents::MOUSE_CLICK;
		}
		break;

	case UIEvents::MOUSE_CLICK:
		if (!OnClick())
		{
			mouse_state = UIEvents::MOUSE_HOVER;
		}
		break;

	default:
		break;
	}

	return true;
}