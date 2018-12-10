#ifndef __j1GUI_H__
#define __j1GUI_H__
#include "p2List.h"
#include "p2Log.h"
#include "j1Module.h"
#include "p2DynArray.h"

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
	
	// TODO 2: Create the factory methods
	// Gui creation functions
	SDL_Texture* GetAtlas() const;
	
public:
	p2List<j1UI_Element*> elements;

private:
	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
};
#endif // __j1GUI_H__ 