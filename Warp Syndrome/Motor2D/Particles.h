#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Animations.h"
#include "j1Module.h"
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
	PB_STATIC_LOOP,
	PB_DYNAMIC_LOOP,
	PB_STATIC_COLLIDABLE_LOOP,
	PB_DYNAMIC_COLLIDABLE_LOOP,
	PB_UNKNOWN
};
struct Particle
{
	//TODO collider
	Animations* anim;
	FrameInfo* currentframe;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	Particle_behaviour behaviour;
	p2SString name;
	bool fx_played = false;
	bool fliped = false;
	//=============================
	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1ParticleManager:public j1Module
{
public:
	j1ParticleManager();
	virtual ~j1ParticleManager();

	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y, bool fliped, Uint32 delay = 0);

private:
	

};

void AddParticle(const Particle& particle, int x, int y, bool fliped, Uint32 delay = 0)
{

}











#endif // !__PARTICLES_H__
