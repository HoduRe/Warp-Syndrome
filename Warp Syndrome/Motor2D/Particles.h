#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animations.h"
#include "Entity.h"



class Particle:public Entity
{
public:
	fPoint offset;
	fPoint gravityaccel;
	fPoint forces;

	float mass;

	//=============================
	Particle(fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan = 1.0f, fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f }, SDL_Rect aTextureSection = { 0,0,0,0 },EntityType type=E_TYPE_PARTICLE);
	Particle(fPoint pPos, SDL_Texture* pTexture, float aLifespan = 1.0f,fPoint aGravity = { 0.0f,0.0f }, fPoint aOffset = { 0.0f,0.0f }, SDL_Rect aTextureSection = { 0,0,0,0 }, EntityType type = E_TYPE_PARTICLE);
	virtual ~Particle();
	virtual void Update();
	void Integrate();
	virtual void PostUpdate();
	void ApplyForce(fPoint aForce);
	void ApplyGravity(fPoint aGravity);

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

	virtual void Update();
	virtual void PostUpdate();

};

#endif // !__PARTICLES_H__
