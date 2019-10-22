#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "transitions.h"

#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL.h"

struct level
{
	p2SString overworld;
	p2SString nether;
	bool hasnether;
};

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

	bool RestartLevel();
	bool ChangeToNextLevel();

	//Save/Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool LoadLevelList(pugi::xml_node& root);

public:

	//map variables================================
	p2SString path;
	p2List<level*> level_list;
	p2List_item<level*>* current_level;
	p2List_item<level*>* default_level;
	uint numoflevels;


	pugi::xml_document leveldoc;
	pugi::xml_node rootnode;
	p2SString filename;
	Transition_States restart_states;


};

#endif // !__LEVEL_MANAGER_H__
