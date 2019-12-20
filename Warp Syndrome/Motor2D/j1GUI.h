#ifndef __j1UIMANAGER_H__
#define __j1UIMANAGER_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "UI_Element.h"
#include "button.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
class j1GUI : public j1Module
{
public:

	j1GUI();

	// Destructor
	virtual ~j1GUI();

	// Called after initializing module
	bool Start();

	// Called before render is available
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called render has been executed
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	p2List<UI*> UI_list;
};

#endif // __j1UIMANAGER_H__
