#include "Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Animations.h"
#include "j1Player.h"
#include"j1Input.h"
//particle functions======================================================================
	//dynamic particle constructor
Particle::Particle(fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, SDL_Rect aTextureSection)
{
	offset = aOffset;
	position.x = pPos.x + offset.x;
	position.y = pPos.y + offset.y;
	speed = pSpeed;
	mass = aMass;
	texture = pTexture;
	lifespan = aLifespan;
	gravityaccel = aGravity;
	texturesection = aTextureSection;
	fPoint nullgravity = { 0.0f,0.0f };
	if (gravityaccel == nullgravity)//if the input gravity is 0, set the world gravity instead of a local one
		gravityaccel = App->particle_m->GetGravity();

	forces = { 0,0 };
	fliped = false;
}
//static particle constructor
Particle::Particle(fPoint pPos, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset, SDL_Rect aTextureSection)
{
	offset = aOffset;
	position.x = pPos.x + offset.x;
	position.y = pPos.y + offset.y;
	texture = pTexture;
	lifespan = aLifespan;
	gravityaccel = aGravity;
	texturesection = aTextureSection;
	fPoint nullgravity = { 0.0f,0.0f };
	if (gravityaccel == nullgravity)//if the input gravity is 0, set the world gravity instead of a local one
		gravityaccel = App->particle_m->GetGravity();

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

	position.x += speed.x;
	position.y += speed.y;

	forces = { 0,0 };//resets forces to 0

}

//called every update
void Particle::ParticleUpdate()
{
	Integrate();
	lifespan--;
}

void Particle::Display()
{
	if (SDL_RectEmpty(&texturesection))
		App->render->Blit(texture, position.x, position.y, NULL, fliped);

	else App->render->Blit(texture, position.x, position.y, &texturesection, fliped);

}

void Particle::ApplyForce(fPoint aForce)
{
	forces += aForce;
}
void Particle::ApplyGravity(fPoint aGravity)
{
	gravityaccel = aGravity;
}

bool Particle::IsDead()
{
	bool ret = false;
	if (lifespan <= 0)
	{
		ret = true;
	}
	return ret;
}

//animated particle functions=============================================================
	//static particle constructor
AnimatedParticle::AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset) :Particle(pPos, pTexture, aLifespan, aGravity, aOffset)
{

	p2List<Animations*>* list = App->player->GetAnimationList();
	p2List_item<Animations*>* item = list->start;
	item = item->data->GetAnimFromName(aAnimName, list);
	anim = *item->data;

	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim.SetLoopable(false);
		lifespan = 1;
	}
	else anim.SetLoopable(true);
}
//dynamic particle constructor
AnimatedParticle::AnimatedParticle(p2SString aAnimName, bool aDieOnEndAnim, fPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan, fPoint aGravity, fPoint aOffset) :Particle(pPos, pSpeed, aMass, pTexture, aLifespan, aGravity, aOffset)
{
	p2List<Animations*>* list = App->player->GetAnimationList();
	p2List_item<Animations*>* item = list->start;
	item = item->data->GetAnimFromName(aAnimName, list);
	anim = *item->data;

	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim.SetLoopable(false);
		lifespan = 1;
	}
	else anim.SetLoopable(true);
}
AnimatedParticle::~AnimatedParticle()
{
}

//called every update
void AnimatedParticle::ParticleUpdate()
{
	Integrate();
	anim.StepAnimation();

	if (dieOnEndAnim && anim.GetAnimationFinish())//if the aprticle dies when its animation finishes change its state to death
		lifespan = 0;

	else if (!dieOnEndAnim)//if the particle dies of age lower its life
		lifespan--;


}

void AnimatedParticle::Display()
{
	FrameInfo* currframe = anim.GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &currframe->animationRect, fliped);
}



//particle manager functions==============================================================
j1ParticleManager::j1ParticleManager()
{
	gravity = { 0.0f,0.2f };//TODO ASSIGN THIS FROM AN XML
}
j1ParticleManager::~j1ParticleManager()
{
	CleanUp();
}

bool j1ParticleManager::Start()
{
	return true;
}
bool j1ParticleManager::Start(fPoint aGravity)
{
	gravity = aGravity;
	return true;
}

// Called before all Updates
bool j1ParticleManager::PreUpdate()
{
	//deletes all the death particles
	if (particles.count() > 0)//if there are particles in the list
	{
		p2List_item<Particle*>* item = particles.start;
		while (item != NULL)
		{
			if (item->data->IsDead())
			{
				particles.del(item);
			}

			item = item->next;
		}

	}


	return true;
}

// Called each loop iteration
bool j1ParticleManager::Update(float dt)
{
	p2List_item<Particle*>* item = nullptr;

	item = particles.start;

	while (item != NULL)//Updates all particles
	{

		item->data->ParticleUpdate();
		item = item->next;
	}
	return true;
}

bool j1ParticleManager::PostUpdate()
{
	p2List_item<Particle*>* item;
	item = particles.start;

	while (item != NULL)//Draws all particles
	{
		item->data->Display();
		item = item->next;
	}
	return true;
}

// Called before quitting
bool j1ParticleManager::CleanUp()
{
	p2List_item<Particle*>* item = particles.start;
	particles.clear();
	return true;
}

//adds a particle to the list
void j1ParticleManager::AddParticle(Particle* particle)
{
	particles.add(particle);
}

//deletes a particle from the list
bool j1ParticleManager::DeleteParticle(Particle* particle)
{
	bool ret = false;

	p2List_item<Particle*>* item = particles.start;
	while (item != NULL)
	{
		if (item->data == particle)//if theres a coincidence in the list
		{
			ret = true;
			particles.del(item);
		}

		item = item->next;
	}
	return ret;
}

fPoint j1ParticleManager::GetGravity()
{
	return gravity;
}
void j1ParticleManager::SetGravity(fPoint aGravity)
{
	gravity = aGravity;
}