#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2List.h"
#include "p2Log.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1UI_Element.h"
#include "j1Button.h"
#include "j1ButtonText.h"

enum BUTTON_TYPE;
enum TEXT_TYPE;

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
	bool PreUpdate(float dt);
	bool Update(float dt);
	// Called after all Updates
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();
	
	//functions to get SDL_Textures easily
	SDL_Texture* GetAtlas() const;
	SDL_Texture* GetMenuBackground() const;

	//void SpawnMenuBackground(int x, int y, E_TYPE type, j1Module* mod);
	j1UI_Element* SpawnButton(int x, int y, BUTTON_TYPE type);
	j1UI_Element* SpawnText(int x, int y, TEXT_TYPE type, uint font, const char* text, SDL_Color color);
	
public:
	p2List<j1UI_Element*> element_list;
	SDL_Texture* menuBackgroundTex = nullptr;

	bool mouse_hovering = false;

private:
	
	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
};
#endif // __j1GUI_H__ 