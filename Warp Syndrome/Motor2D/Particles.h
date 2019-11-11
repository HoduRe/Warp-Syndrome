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
	fPoint position;
	fPoint offset;
	fPoint speed;
	fPoint gravityaccel;
	fPoint forces;
	SDL_Texture* texture;
	SDL_Rect texturesection;

	int lifespan;
	float mass;
	bool fliped;

	//=============================
	Particle(fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan = 1.0f, fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f }, SDL_Rect aTextureSection = { 0,0,0,0 });
	Particle(fPoint pPos, SDL_Texture* pTexture, float aLifespan = 1.0f,fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f }, SDL_Rect aTextureSection = { 0,0,0,0 });
	virtual ~Particle();
	virtual void ParticleUpdate();
	void Integrate();
	virtual void Display();
	void ApplyForce(fPoint aForce);
	void ApplyGravity(fPoint aGravity);
	bool IsDead();

};

class AnimatedParticle :public Particle
{
public:
	Animations anim;
	bool dieOnEndAnim;

	//=============================
	AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan = 1.0f, fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f });
	AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, SDL_Texture* pTexture, float aLifespan=1.0f,fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f });
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
public:
	fPoint gravity;

private:
	p2List<Particle*> particles;
	
};

#endif // !__PARTICLES_H__
