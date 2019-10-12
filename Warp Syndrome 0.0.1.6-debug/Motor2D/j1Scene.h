#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

enum MapLoaded
{
	NOLEVEL,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4
};
struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

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

	void RepositionCamera();

private:
	bool reload = false;
	MapLoaded currentlevel=LEVEL1; //TODO make a new module to store the different levels and their relation with functions to change to a different level with fadeout
};

#endif // __j1SCENE_H__