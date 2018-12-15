#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2List.h"
#include "p2Log.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1UI_Element.h"
#include "j1Button.h"
#include "j1ButtonText.h"
#include "j1Fonts.h"

enum BUTTON_TYPE;
enum TEXT_TYPE;
enum FONT_TYPE;

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
	j1UI_Element* SpawnText(int x, int y, uint font, E_TYPE type, const char* text, SDL_Color color);
	j1UI_Element * SpawnImage(int x, int y, SDL_Rect rect, bool visible);
	
public:
	p2List<j1UI_Element*> element_list;
	SDL_Texture* menuBackgroundTex = nullptr;
	//p2DynArray<_TTF_Font*> fonts_array;

	bool mouse_hovering = false;
	bool startgame = false;

private:
	
	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
};
#endif // __j1GUI_H__ 