#ifndef __j1BUTTONTEXT_H__
#define __j1BUTTONTEXT_H__

#include "j1Gui.h"
#include "j1UI_Element.h"

class SDL_Texture;
class _TTF_Font;


class j1ButtonText : public j1UI_Element {
	public: 
		j1ButtonText(uint font, E_TYPE type, const char * text, SDL_Color color);
		j1ButtonText();

		bool Start();
		bool PostUpdate();

		void SelectFont();

	public:

		const char* string = nullptr;
		SDL_Color text_color = { 102, 51, 0, 255 };
		SDL_Texture* text_texture = nullptr;
		//FONT_TYPE font_type;
		uint font;

};
#endif // __j1BUTTONTEXT_H__
