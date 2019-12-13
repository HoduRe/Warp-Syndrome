#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include "brofiler/Brofiler/Brofiler.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Collision;
class j1LevelManager;
class j1Transitions;
class j1ParticleManager;
class j1PathFinding;
class j1EntityManager;
class j1Console;
class j1UIManager;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;
	void ToggleCapping();
private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;
	j1Collision*		collision;
	j1LevelManager*		level_m;
	j1Transitions*		transitions;
	j1PathFinding*		pathfinding;
	j1EntityManager*	entity_m;
	j1Console*			console;
	j1UIManager*		UI_m;


	float				dt;
	j1Timer				frame_time;
	bool				capping = false;
	bool				vSyncActivated = false;
	bool				displayMapInfo = false;
private:

	p2List<j1Module*>	modules;
	uint				frames;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;
	uint32				new_max_framerate;
	uint32				prev_last_sec_frame_count = 0;
	uint32				last_sec_frame_count = 0;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				last_sec_frame_time;


};

extern j1App* App; 

#endif