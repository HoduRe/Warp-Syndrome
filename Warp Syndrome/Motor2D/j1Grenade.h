#ifndef __j1GRENADE_H__
#define __j1GRENADE_H__

#include "j1Module.h"
#include "p2List.h"

class j1Grenade : public j1Module
{
public:

	j1Grenade();

	// Destructor
	virtual ~j1Grenade();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();


private:

};

#endif // __j1GRENADE_H__