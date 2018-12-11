#ifndef __j1BUTTONTEXT_H__
#define __j1BUTTONTEXT_H__

#include "j1UI_Element.h"

struct j1Module;
struct _TTF_Font;
enum E_TYPE;

class j1ButtonText : public j1UI_Element {

public:
	
	j1ButtonText(int x, int y, E_TYPE type, j1Module* mod, const char* text, _TTF_Font* font);
	virtual ~j1ButtonText();

	void Draw();

public:
	_TTF_Font* font;
	const char* text;
};

#endif // __j1BUTTONTEXT_H__
