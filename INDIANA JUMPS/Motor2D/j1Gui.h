#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2List.h"
#include "p2Log.h"
#include "j1Module.h"
#include "p2DynArray.h"

struct j1Button;
class SDL_Texture;
class SDL_Rect;
class j1UI_Element;
enum E_TYPE;
class _TTF_Font;

enum UIEvents
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK,
	MOUSE_STOP_CLICK
};

class j1Gui : public j1Module
{
public:
	j1Gui();
	// Destructor
	virtual ~j1Gui();
	// Called when before render is available
	bool Awake(pugi::xml_node&);
	// Call before first frame
	bool Start();
	// Called before all Updates
	bool PreUpdate();
	//bool Update(float dt);
	// Called after all Updates
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();
	
	//functions to get SDL_Textures easily
	SDL_Texture* GetAtlas() const;
	SDL_Texture* GetMenuBackground() const;

	//void SpawnMenuBackground(int x, int y, E_TYPE type, j1Module* mod);
	j1UI_Element* SpawnButton(int x, int y, E_TYPE type, SDL_Rect* rect, j1Module* mod, bool visible);
	//j1UI_Element* SpawnButtonText(int x, int y, E_TYPE type, j1Module* mod, const char* text, _TTF_Font* font);
	
public:
	p2List<j1UI_Element*> element_list;
	SDL_Texture* menuBackgroundTex = nullptr;

private:
	
	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
};
#endif // __j1GUI_H__ 