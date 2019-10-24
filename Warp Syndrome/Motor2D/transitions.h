#ifndef __TRANSITIONS_H__
#define __TRANSITIONS_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

enum Transition_States
{
	TS_START,
	TS_FADE_OUT,
	TS_LOADING_START,
	TS_BLACK_SCREEN,
	TS_LOADING_PROCESS,
	TS_LOADING_FINISH,
	TS_FADE_IN,
	TS_FINISHED,
	TS_UNKNOWN
};
enum Transition_Mode
{
	TM_RESTART_LEVEL,
	TM_CHANGE_TO_NEXT_LEVEL,
	TM_UNKNOWN
};

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

	//Fades into black in a gradient for the amount of frames you input
	bool Fade_In(uint frames_length);

	//Fades out of black in a gradient for the amount of frames you input
	bool Fade_Out(uint frames_length);

	//Runs the animation for the loading screen
	bool LoadingScreen(uint frames_length);

	bool BlackScreen(uint frames_length);
	
	bool BlackScreen();

	void ChangeTransition(Transition_Mode mode, uint frames_length);

public:

	Transition_States actual_state;
	Transition_Mode actual_transition;
	bool transitionended = false;

private:
	
	uint timer;
	iPoint textcenter;
	iPoint symbolcenter;

	uint function_frames_length;

	SDL_Texture* loadingText = nullptr;
	SDL_Texture* externalLogo = nullptr;
	SDL_Texture* internalLogo = nullptr;
	SDL_Texture* hexagonLogo = nullptr;
	SDL_Texture* imageLogo = nullptr;
	float degreesperframe = 1; //TODO Load this from scene.xml
	float degrees = 0;
	int transition = 0;
	int currenttime = 0;
	bool fadeended = false;

};



#endif // !__TRANSITIONS_H__

