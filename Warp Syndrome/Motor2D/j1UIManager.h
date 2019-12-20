#ifndef __j1UIMANAGER_H__
#define __j1UIMANAGER_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "UI.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
class j1UIManager : public j1Module
{
public:

	j1UIManager();

	// Destructor
	virtual ~j1UIManager();

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
