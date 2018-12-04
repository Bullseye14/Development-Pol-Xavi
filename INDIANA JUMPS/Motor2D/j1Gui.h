#ifndef __GUI_H__
#define __GUI_H__

#include "j1App.h"
#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1UI_Element.h"

class j1UI_Element;

class j1Gui : public j1Module 
{
public:
	
	j1UI_Element * CreateElement(int x, int y, E_TYPE type);


	p2List<j1UI_Element*> element_list;
};

#endif // __GUI_H__