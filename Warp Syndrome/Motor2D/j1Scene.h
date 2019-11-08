#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animations.h"//TODO delete when stopeed debugging particles

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

	void LoadNewLevel(iPoint textcenterpos, iPoint symbolcenterpos);

	float CameraGoToTarget(SDL_Rect camera, fPoint target);//used inside Repositon Camera Function

	int teleport_sfx;
	int jump_sfx;
	int death_sfx;

private:
	bool start;

	//Reposition Camera Variables========================================================================
	fPoint camaccel;
	bool targetRight;//true if Right, false if Left
	bool arrivedtoline;//true if the camera has arrived to its desired position, false if else
	bool snapping;//true if its currently snapping false if else 
	fPoint distancetoplayer;//camera distance to player

	//TODO delete when stopeed debugging particles===============================
	SDL_Texture* parttexture;
	SDL_Texture* partanimtexture;
	Animations animat;

};

#endif // __j1SCENE_H__