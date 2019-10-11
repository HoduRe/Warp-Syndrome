//this module is "Work in Progress", might get deleted later on
#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__
#include "p2List.h"
#include "p2Point.h"
#include "p2SString.h"
#include "SDL/include/SDL_rect.h"
//stores Info about the current frame of the animation
struct FrameInfo
{
	int frameduration;//duration of the current frame in loops
	SDL_Rect animationRect; //rectangle with the location of the frame in the spritesheet
	iPoint textureoffset;//texture offset from the source position
	//TODO finish working on the data stored in here
};
//stores Info about a single animation and all its frames
class Animation
{
public:
	Animation();
	~Animation();
	//adds a new frame to the animation
	void AddFrame(int duration, SDL_Rect texturerect, iPoint textureoffset);
	//returns the current frame of the animation and steps to the net one
	FrameInfo* StepAnimation();
	//returns true if the animation has finished
	bool GetAnimationFinish();
private:
	p2SString animationname; //name of the current animation
	bool animationloop; //0 if the animation cannot loop / 1 if the animation can be looped
	int numberofFrames; //Contains information about the number of frames this animation has
	p2List_item<FrameInfo*>* currentanimframe;//pointer to the current frame of the animation(TODO can be replaced if we use p2list which has index)	
	p2List<FrameInfo*> animationframes; //list containing all the frames of the animation
	bool animationfinished; 
   //TODO finish working on the data stored in here

};

Animation::Animation()
{
	currentanimframe = animationframes.start;
}

Animation::~Animation()
{
	delete currentanimframe;
	animationframes.clear();

}

void Animation::AddFrame(int duration, SDL_Rect texturerect, iPoint textureoffset)
{
	FrameInfo* newframe = new FrameInfo;

	newframe->frameduration = duration;
	newframe->textureoffset = textureoffset;
	newframe->animationRect = texturerect;

	animationframes.add(newframe);
	numberofFrames++;
}

FrameInfo* Animation::StepAnimation()
{
	FrameInfo* ret = currentanimframe->data;

	if (currentanimframe->next != nullptr)//if the next element exists go to the next element
		currentanimframe = currentanimframe->next;
	else if (animationloop)//if the animation can loop return to the start
		currentanimframe = animationframes.start;


	return ret;
}
#endif // !__ANIMATIONS_H__
