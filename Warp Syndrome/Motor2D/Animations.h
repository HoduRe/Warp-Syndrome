#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__
#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
//this module is "Work in Progress", might get deleted later on

#include "p2Point.h"
#include "p2SString.h"
#include "p2List.h"

enum Animation_state
{
	AS_UNFINISHED,
	AS_FINISHED,
	AS_UNKNOWN
};

//stores Info about the current frame of the animation
struct FrameInfo
{
	//TODO frameduration and actualduration have to be modified to work with the game actual framerate instead of loops

	float frameduration;//duration of the current frame in loops
	float actualduration; //how many units of duration have passed since the first frame
	SDL_Rect animationRect; //rectangle with the location of the frame in the spritesheet
	iPoint textureoffset;//texture offset from the source position
};
//stores Info about a single animation and all its frames
class Animations
{
public:
	Animations();
	Animations(p2SString aName,bool aLoop, int aNumberOfFrames);
	~Animations();
	bool CleanUp();


	//adds a new frame to the animation
	void AddFrame(float duration, SDL_Rect texturerect, iPoint textureoffset);
	
	//returns the current frame of the animation and steps to the net one
	FrameInfo* StepAnimation(float dt);
	//returns the current frame of the animation and steps to the net one
	FrameInfo* StepAnimation(Animation_state& state,float dt);
	
	//returns true if the animation has finished
	bool GetAnimationFinish();

	//returns the frame with the given id in the frame list
	FrameInfo* GetFrame(int id);
	
	FrameInfo* GetCurrentFrame();

	//loads an animation from an animation node
	//returns true if loaded correctly, false, if the number of frames loaded don't correspond to the one specified in the .xml 
	bool LoadAnim(pugi::xml_node& animationnode);

	p2List_item<Animations*>* GetAnimFromName(p2SString name, p2List<Animations*>* animlist);

	void ResetAnimation();

public:
	bool animationloop; //0 if the animation cannot loop / 1 if the animation can be looped
	p2SString animationname; //name of the current animation
private:
	int numberofFrames; //Contains information about the number of frames this animation has
	p2List_item<FrameInfo*>* currentanimframe;//pointer to the current frame of the animation	
	p2List<FrameInfo*> animationframes; //list containing all the frames of the animation
	bool animationfinished; 
   
};

#endif // !__ANIMATIONS_H__
