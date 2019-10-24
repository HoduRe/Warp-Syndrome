#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Animations.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Textures.h"

enum Particle_behaviour
{
	PB_STATIC,
	PB_DYNAMIC,
	PB_STATIC_COLLIDABLE,
	PB_DYNAMIC_COLLIDABLE,
	PB_UNKNOWN
};

class Particles
{
public:
	Particles();
	Particles(p2SString name,iPoint pos, fPoint vel, int lifetime,SDL_Texture* texture, bool fliped);
	~Particles();

private:
	p2SString name;
	Animations* anim;
	FrameInfo* currentframe;
	iPoint pos;
	fPoint vel;
	int lifetime_in_frames;//-1 to be inmortal
	SDL_Texture* texture;
	bool fliped;

};

Particles::Particles()
{
	name = "";
	anim = nullptr;
	pos = { 0,0 };
	vel = { 0,0 };
	lifetime_in_frames=-1;
}
Particles::Particles(p2SString pname, iPoint ppos, fPoint pvel, int plifetime, SDL_Texture* ptexture, bool pfliped)
{
	name = pname;
	pos = ppos;
	vel = pvel;
	lifetime_in_frames = plifetime;
	texture = ptexture;
	fliped = pfliped;
}

Particles::~Particles()
{
	App->tex->UnLoad(texture);
	delete anim;
}













#endif // !__PARTICLES_H__
