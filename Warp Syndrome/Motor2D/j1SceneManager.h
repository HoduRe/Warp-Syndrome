#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"
#include "UI_Elements.h"


class UI;
enum UICallbackState;

enum GameCycle
{
	G_C_START,
	G_C_MAIN_MENU,
	G_C_PAUSE_MENU,
	G_C_INGAME,
	G_C_UNKNOWN
};

class j1SceneManager : public j1Module
{
public:

	j1SceneManager();

	// Destructor
	virtual ~j1SceneManager();

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
	bool OnListen(UI* element, UICallbackState cstate);

	bool LoadMainMenu();
	bool UnloadMainMenu();
	bool LoadPauseMenu();
	bool UnloadPauseMenu();
	bool LoadHUD();
	bool UnloadHUD();

	
public:
	GameCycle currentloop;
	bool doingaction = false;//bool active means the loop is already performing an action/transition
};

#endif // __j1SCENEMANAGER_H__