#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__
#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#include "SDL/include/SDL.h"


class j1LevelManager : public j1Module
{
public:
	j1LevelManager();
	virtual ~j1LevelManager();
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

#endif // !__LEVEL_MANAGER_H__
