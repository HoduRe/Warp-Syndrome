#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "Animations.h"
#include "Entity.h"

class Character :public Entity
{
public:
	Character(EntityType atype = E_TYPE_CHARACTER);
	~Character();

	//steps the current animation and returns an Aniamtion state(finished, unfinished, unknown)
	Animation_state StepCurrentAnimation();

	//loads animations from an xml
	bool LoadAnimations(pugi::xml_node& rootnode);

	//flips the character
	virtual bool FlipCharacter(fPoint currentpos, fPoint lastpos);

public:
	FrameInfo* currentframe;
	p2List_item<Animations*>* currentAnim;
	iPoint hitbox_w_h;
	p2List<Animations*> animations_list;

};

#endif // !__CHARACTER_H__

