#include "Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Animations.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "Player.h"
//particle functions======================================================================
	//default particle constructor
Particle::Particle(EntityType type) :Entity(type)
{}

//dynamic particle constructor
Particle::Particle(fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, SDL_Rect aTextureSection, EntityType type) : Entity(type)
{
	offset = aOffset;
	pos.x = pPos.x + offset.x;
	pos.y = pPos.y + offset.y;
	speed = pSpeed;
	mass = aMass;
	texture = pTexture;
	health = aLifespan;
	gravityaccel = aGravity;
	texture_section = aTextureSection;
	fPoint nullgravity = { 0.0f,0.0f };
	if (gravityaccel == nullgravity)//if the input gravity is 0, set the world gravity instead of a local one
		gravityaccel = App->entity_m->gravity;

	forces = { 0,0 };
	fliped = false;
}
//static particle constructor
Particle::Particle(fPoint pPos, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, SDL_Rect aTextureSection, EntityType type) :Entity(type)
{
	offset = aOffset;
	pos.x = pPos.x + offset.x;
	pos.y = pPos.y + offset.y;
	texture = pTexture;
	health = aLifespan;
	gravityaccel = aGravity;
	texture_section = aTextureSection;
	fPoint nullgravity = { 0.0f,0.0f };
	if (gravityaccel == nullgravity)//if the input gravity is 0, set the world gravity instead of a local one
		gravityaccel = App->entity_m->gravity;

	speed = { 0,0 };
	forces = { 0,0 };
	mass = 0.0f;
	fliped = false;
}
Particle::~Particle()
{
}

//position integrator
void Particle::Integrate()
{
	//Euler Integration===============================
	fPoint accel = { 0,0 };
	if (mass != 0.0f)
	{
		accel.x = forces.x / mass;
		accel.y = forces.y / mass;
		accel.x += gravityaccel.x;//add gravity
		accel.y += gravityaccel.y;
	}


	speed.x += accel.x;
	speed.y += accel.y;

	pos.x += speed.x;
	pos.y += speed.y;

	forces = { 0,0 };//resets forces to 0

}

//called every update
bool Particle::Update(float dt)
{
	Integrate();
	health--;
	return true;
}

bool Particle::PostUpdate()
{
	if (SDL_RectEmpty(&texture_section))
		App->render->Blit(texture, pos.x, pos.y, NULL, fliped);

	else App->render->Blit(texture, pos.x, pos.y, &texture_section, fliped);

	if (health <= 0)
		destroy = true;
	return true;
}

void Particle::ApplyForce(fPoint aForce)
{
	forces += aForce;
}
void Particle::ApplyGravity(fPoint aGravity)
{
	gravityaccel = aGravity;
}


//animated particle functions=============================================================
	//default particle constructor
AnimatedParticle::AnimatedParticle(EntityType type) :Particle(type)
{
	dieOnEndAnim = true;
}
//static particle constructor
AnimatedParticle::AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, EntityType type) : Particle(pPos, pTexture, aLifespan, aGravity, aOffset, { 0,0,0,0 }, type)
{

	p2List<Animations*>* list = &App->entity_m->player->animations_list;
	p2List_item<Animations*>* item = list->start;
	item = item->data->GetAnimFromName(aAnimName, list);
	anim = *item->data;

	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim.animationloop = false;
		health = 1;
	}
	else anim.animationloop = true;
}
//dynamic particle constructor
AnimatedParticle::AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, EntityType type) :Particle(pPos, pSpeed, aMass, pTexture, aLifespan, aGravity, aOffset, { 0,0,0,0 }, type)
{
	p2List<Animations*>* list = &App->entity_m->player->animations_list;
	p2List_item<Animations*>* item = list->start;
	item = item->data->GetAnimFromName(aAnimName, list);
	
	if (item != NULL)
		anim = *item->data;



	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim.animationloop = false;
		health = 1;
	}
	else anim.animationloop = true;
}
AnimatedParticle::~AnimatedParticle()
{
}


//called every update
bool AnimatedParticle::Update(float dt)
{
	Integrate();
	anim.StepAnimation();

	if (dieOnEndAnim && anim.GetAnimationFinish())//if the aprticle dies when its animation finishes change its state to death
		health = 0;
	else if (!dieOnEndAnim)//if the particle dies of age lower its life
		health--;

	return true;
}

bool AnimatedParticle::PostUpdate()
{
	FrameInfo* currframe = anim.GetCurrentFrame();
	App->render->Blit(texture, pos.x, pos.y, &currframe->animationRect, fliped);
	if (health <= 0)
		destroy = true;

	return true;
}