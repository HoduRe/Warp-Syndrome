#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__
#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
//this module is "Work in Progress", might get deleted later on

#include "p2Point.h"
#include "p2SString.h"
#include "p2List.h"

//stores Info about the current frame of the animation
struct FrameInfo
{
	int frameduration;//duration of the current frame in loops
	SDL_Rect animationRect; //rectangle with the location of the frame in the spritesheet
	iPoint textureoffset;//texture offset from the source position
	//TODO finish working on the data stored in here
};
//stores Info about a single animation and all its frames
class Animations
{
public:
	Animations();
	~Animations();

	//adds a new frame to the animation
	void AddFrame(int duration, SDL_Rect texturerect, iPoint textureoffset);
	
	//returns the current frame of the animation and steps to the net one
	FrameInfo* StepAnimation();
	
	//returns true if the animation has finished
	bool GetAnimationFinish();

	//returns the frame with the given id in the frame list
	FrameInfo* GetFrame(int id);

	//loads an animation from an animation node
	//returns true if loaded correctly, false, if the number of frames loaded don't correspond to the one specified in the .xml 
	bool LoadAnim(pugi::xml_node& animationnode);

private:
	p2SString animationname; //name of the current animation
	//SDL_Texture* texture;//pointer to the animation spritesheet
	bool animationloop; //0 if the animation cannot loop / 1 if the animation can be looped
	int numberofFrames; //Contains information about the number of frames this animation has
	p2List_item<FrameInfo*>* currentanimframe;//pointer to the current frame of the animation(TODO can be replaced if we use p2list which has index)	
	p2List<FrameInfo*> animationframes; //list containing all the frames of the animation
	bool animationfinished; 
   
	//TODO finish working on the data stored in here

};

Animations::Animations()
{
	currentanimframe = animationframes.start;
	animationfinished = 0;
	numberofFrames = 0;
	animationloop = false;
	//texture = nullptr;
}

Animations::~Animations()
{
	delete currentanimframe;
	animationframes.clear();

}

void Animations::AddFrame(int duration, SDL_Rect texturerect, iPoint textureoffset)
{
	FrameInfo* newframe = new FrameInfo;

	newframe->frameduration = duration;
	newframe->textureoffset = textureoffset;
	newframe->animationRect = texturerect;

	animationframes.add(newframe);
	numberofFrames++;
}

FrameInfo* Animations::StepAnimation()
{
	FrameInfo* ret = currentanimframe->data;

	if (currentanimframe->next != nullptr)//if the next element exists go to the next element
		currentanimframe = currentanimframe->next;
	else if (animationloop)//if the animation can loop return to the start
		currentanimframe = animationframes.start;
	else animationfinished = true;

	return ret;
}

bool Animations::GetAnimationFinish()
{
	if (animationfinished)
	{
		animationfinished = 0;
		return true;
	}
	return false;
}

FrameInfo* Animations::GetFrame(int id)
{
	return animationframes.At(id)->data;
}

bool Animations::LoadAnim(pugi::xml_node& animationnode)
{
	pugi::xml_node framenode;
	animationname = animationnode.attribute("name").as_string();
	for (framenode = animationnode.child("frame"); framenode; framenode = framenode.next_sibling("frame"))
	{
		int duration = framenode.attribute("duration").as_int();

		SDL_Rect rect;
		rect.x = framenode.child("rectangle").attribute("x").as_int();
		rect.y = framenode.child("rectangle").attribute("y").as_int();
		rect.w = framenode.child("rectangle").attribute("w").as_int();
		rect.h = framenode.child("rectangle").attribute("h").as_int();
		iPoint textureoffset;
		textureoffset.x = framenode.child("pivot").attribute("x").as_int();
		textureoffset.y = framenode.child("pivot").attribute("y").as_int();

		AddFrame(duration, rect, textureoffset);
	}

	//checks if the number of frames loaded corresponds to the number in the xml, if not return false.
	if (numberofFrames != animationnode.attribute("numberofframes").as_int())
		return false;

	return true;

}

#endif // !__ANIMATIONS_H__
