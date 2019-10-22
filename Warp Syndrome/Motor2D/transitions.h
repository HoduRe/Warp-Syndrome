#ifndef __TRANSITIONS_H__
#define __TRAMSITIONS_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

class j1Transitions : public j1Module
{
public:
	j1Transitions();
	virtual ~j1Transitions();
	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Save/Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

};



#endif // !__TRANSITIONS_H__

