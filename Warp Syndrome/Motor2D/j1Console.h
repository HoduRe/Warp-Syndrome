#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
class j1Console : public j1Module
{
public:

	j1Console();

	// Destructor
	virtual ~j1Console();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

};

#endif // __j1CONSOLE_H__