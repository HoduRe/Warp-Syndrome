#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animations.h"



class Particle
{
public:
	iPoint position;
	fPoint speed;
	fPoint forces;
	SDL_Texture* texture;
	
	int lifespan;
	float mass;
	bool fliped;

	//=============================
	Particle(iPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan = 1.0f);
	Particle(iPoint pPos, SDL_Texture* pTexture, float aLifespan = 1.0f);
	virtual ~Particle();
	virtual void ParticleUpdate();
	void Integrate();
	virtual void Display();
	void ApplyForce(fPoint aForce);
	bool IsDead();

};

class AnimatedParticle :public Particle
{
public:
	Animations* anim;
	FrameInfo* currentframe;
	bool dieOnEndAnim;

	//=============================
	AnimatedParticle(Animations aAnim, bool aDieOnEndAnim, iPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan);
	AnimatedParticle(Animations aAnim, bool aDieOnEndAnim, iPoint pPos, SDL_Texture* pTexture, float aLifespan);
	virtual~AnimatedParticle();

	void ParticleUpdate();
	void Display();

};


class j1ParticleManager:public j1Module
{
public:
	j1ParticleManager();

	virtual ~j1ParticleManager();

	bool Start();
	bool Start(fPoint aGravity);

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	void AddParticle(Particle* particle);
	bool DeleteParticle(Particle* particle);


private:
	p2List<Particle*> particles;
	fPoint gravity;
};

#endif // !__PARTICLES_H__
