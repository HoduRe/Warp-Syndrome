#include "j1App.h"
#include "Character.h"
#include "Entity.h"
#include "j1Textures.h"


Character::Character(EntityType atype) :Entity(atype)
{
	currentframe = nullptr;
	currentAnim = nullptr;
	hitbox_w_h = { 0,0 };
	texture = nullptr;
}
Character::~Character()
{
	animations_list.clear();
	currentframe = nullptr;
	currentAnim = nullptr;
	App->tex->UnLoad(texture);
}

Animation_state Character::StepCurrentAnimation(float dt)
{
	Animation_state aState = AS_UNKNOWN;
	p2List_item<Animations*>* currentanim = currentAnim;
	FrameInfo* frame;
	frame = currentanim->data->StepAnimation(aState,dt);
	currentframe = frame;

	return aState;
}

bool Character::LoadAnimations(pugi::xml_node& rootnode)
{
	bool ret = false;
	for (pugi::xml_node animationnode = rootnode.child("animation"); animationnode; animationnode = animationnode.next_sibling("animation"))
	{
		Animations* anim = new Animations;
		ret = anim->LoadAnim(animationnode);

		animations_list.add(anim);
	}
	return ret;
}

bool Character::FlipCharacter(fPoint currentpos, fPoint lastpos)
{
	bool pfliped = fliped;
	if (currentpos.x < lastpos.x)pfliped = true;
	else if (currentpos.x > lastpos.x)pfliped = false;

	return pfliped;
}
