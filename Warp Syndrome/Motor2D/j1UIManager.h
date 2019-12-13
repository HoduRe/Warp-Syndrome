#ifndef __j1UIMANAGER_H__
#define __j1UIMANAGER_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
class j1UIManager : public j1Module
{
public:

	j1UIManager();

	// Destructor
	virtual ~j1UIManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

};

#endif // __j1UIMANAGER_H__
