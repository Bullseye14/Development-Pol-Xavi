#ifndef __j1BUTTONTEXT_H__
#define __j1BUTTONTEXT_H__

#include "j1Gui.h"
#include "j1UI_Element.h"

class SDL_Texture;
class _TTF_Font;

enum TEXT_TYPE 
{
	TEXT_TITLE,
	TEXT_BUTTON
};

class j1ButtonText : public j1UI_Element {
	public: 
		j1ButtonText(int x, int y, TEXT_TYPE type, uint font, const char* text, SDL_Color color);
		j1ButtonText();

	public:

		uint font;
		const char* string = nullptr;
		SDL_Color text_color = { 102, 51, 0, 255 };


};
#endif // __j1BUTTONTEXT_H__
