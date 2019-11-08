#include "Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Animations.h"

//particle functions======================================================================
	//dynamic particle constructor
Particle::Particle(iPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan)
{
	position = pPos;
	speed = pSpeed;
	mass = aMass;
	texture = pTexture;
	lifespan = aLifespan;

	forces = { 0,0 };
	fliped = false;
}
//static particle constructor
Particle::Particle(iPoint pPos, SDL_Texture* pTexture, float aLifespan)
{
	position = pPos;
	texture = pTexture;
	lifespan = aLifespan;

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
	App->render->Blit(texture, position.x, position.y, NULL, fliped);
}

void Particle::ApplyForce(fPoint aForce)
{
	forces += aForce;
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
AnimatedParticle::AnimatedParticle(Animations aAnim, bool aDieOnEndAnim, iPoint pPos, SDL_Texture* pTexture, float aLifespan) :Particle(pPos, pTexture, aLifespan)
{
	anim = new Animations(aAnim);
	currentframe = anim->GetCurrentFrame();
	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim->SetLoopable(false);
		lifespan = 1;
	}
	else anim->SetLoopable(true);
}
//dynamic particle constructor
AnimatedParticle::AnimatedParticle(Animations aAnim, bool aDieOnEndAnim, iPoint pPos, fPoint pSpeed, float aMass, SDL_Texture* pTexture, float aLifespan) :Particle(pPos, pSpeed, aMass, pTexture, aLifespan)
{
	anim = new Animations(aAnim);

	currentframe = anim->GetCurrentFrame();
	dieOnEndAnim = aDieOnEndAnim;
	if (dieOnEndAnim)
	{
		anim->SetLoopable(false);
		lifespan = 1;
	}
	else anim->SetLoopable(true);
}
AnimatedParticle::~AnimatedParticle()
{
	if (anim != nullptr)
		delete anim;
	anim = nullptr;

	if (currentframe != nullptr)
		delete currentframe;
	currentframe = nullptr;
}

//called every update
void AnimatedParticle::ParticleUpdate()
{
	Integrate();
	anim->StepAnimation();

	if (dieOnEndAnim && anim->GetAnimationFinish())//if the aprticle dies when its animation finishes change its state to death
		lifespan = 0;

	else if (!dieOnEndAnim)//if the particle dies of age lower its life
		lifespan--;


}

void AnimatedParticle::Display()
{
	FrameInfo* currframe = anim->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &currframe->animationRect, fliped);
}



//particle manager functions==============================================================
j1ParticleManager::j1ParticleManager()
{
	gravity = { 0,0.1f };//TODO ASSIGN THIS FROM AN XML
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
				RELEASE(item->data);
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
		item->data->ApplyForce(gravity);
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
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	particles.clear();
	return true;
}

//adds a particle to the list
void j1ParticleManager::AddParticle(Particle* particle)
{
	Particle* part = particle;
	particles.add(part);
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
			RELEASE(item->data);
			particles.del(item);
		}

		item = item->next;
	}
	return ret;
}
