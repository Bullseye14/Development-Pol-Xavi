#ifndef __j1MENUBACKGROUND_H__
#define __j1MENUBACKGROUND_H__

#include "j1UI_Element.h"

class j1MenuBackground :public j1UI_Element
{
public:

	j1MenuBackground(int x, int y, E_TYPE type, j1Module* modul);
	~j1MenuBackground();

	void Draw();

public:

};

#endif //__j1MENUBACKGROUND_H__