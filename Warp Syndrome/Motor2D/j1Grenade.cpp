#include "p2Defs.h"
#include "p2Log.h"
#include "j1Grenade.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Particles.h"
#include "Player.h"

//default constructor
j1Grenade::j1Grenade() :AnimatedParticle(EntityType::E_TYPE_GRENADE)
{
	pos = { 0.0f,0.0f };
	speed = { 0.0f,0.0f };
	health = 0.0f,
		fliped = false;
	texture = nullptr;
	texture_section = { 0,0,0,0 };
	destroy = false;
	enabled = true;

	offset = { 0.0f,0.0f };
	gravityaccel = { 0.0f,0.0f };
	forces = { 0.0f,0.0f };
	mass = 1.0f;

	dieOnEndAnim = false;
}
//constructor
j1Grenade::j1Grenade(fPoint aPos, fPoint aSpeed, float aHealth) : AnimatedParticle("grenade", false, aPos, aSpeed, 1.0f, nullptr, aHealth, { 0.0f,0.0f }, { 0.0f,0.0f }, E_TYPE_GRENADE)
{
	pos = aPos;
	speed = aSpeed;
	health = aHealth;
	fliped = false;
	texture = nullptr;
	texture_section = { 0,0,0,0 };
	destroy = false;
	enabled = true;

	offset = { 0.0f,0.0f };
	gravityaccel = { 0.0f,0.0f };
	forces = { 0.0f,0.0f };
	mass = 1.0f;

	dieOnEndAnim = false;
	App->entity_m->grenade = (j1Grenade*)App->entity_m->CreateEntity(EntityType::E_TYPE_GRENADE);

}

// Destructor
j1Grenade::~j1Grenade() 
{
	App->entity_m->grenade = nullptr;
}

// Called before the first frame
bool j1Grenade::Start() {

	texture = App->entity_m->player->texture;
	speed.x = App->entity_m->player->speed.x * 3 / 2;
	speed.y = App->entity_m->player->speed.y;
	return true;
}

// Called each loop iteration
bool j1Grenade::Update(float dt) {

	bool playercantp = true;


	//TODO, multiply velocity component of the granade *-0.9f when collision happens. X for horizontal collisions and Y for vertical ones
	//be careful if grenade was on for example ground collision and enters a ground-right collision not to flip the vertical component again
	//also detect if the collision is happening with a grenade collision object(the ones that grenades can pass through but player can't)
	//and change the playercantp variable to true accordingly (its already implemented that if its true tp doesn't happen)


	App->render->Blit(texture, pos.x, pos.y, &anim.StepAnimation()->animationRect);

	if (health <= 0.0f || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || App->input->GetMouseButtonDown(2) == KEY_DOWN)//explode the granade
		destroy = true;
	else if ((App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN || App->input->GetMouseButtonDown(3) == KEY_DOWN) &&App->entity_m->player->current_state != DYING && playercantp==true) //if the player is not dying, and the grenade hasn't been destroyed can tp
		Teleport();

	return true;
}

// Called before quitting
bool j1Grenade::CleanUp() {
	App->tex->UnLoad(texture);//TODO don't know if its a copy of the player texture or a poniter to it, so maybe, when cleaning the player dissapears :p guess we will find out soon
	return true;
}

void j1Grenade::Teleport()
{
	App->audio->PlayFx(App->scene->teleport_sfx);
	//generate a particle on the player then move the player and generate the other when TPed
	AnimatedParticle* p = new AnimatedParticle("pulsar_in", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, { 0.0f,0.0f }, { -50.0f,-43.0f });
	App->entity_m->AddEntity(p);
	App->entity_m->player->pos = pos;
	AnimatedParticle* q = new AnimatedParticle("pulsar_out", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, { 0.0f,0.0f }, { -50.0f,-43.0f });
	App->entity_m->AddEntity(p);

	destroy = true;
}

void j1Grenade::GrenadeCollisions() 
{

	fPoint measures;
	measures.x = anim.GetCurrentFrame()->animationRect.x;
	measures.x = anim.GetCurrentFrame()->animationRect.x;

	App->collision->CheckLoop(&pos, &measures, OBJECT_GRENADE);

	if (grenade_collider_buffer != App->collision->current_collision) //if the last collision was different from the one happening now enter the switch//bad things can happen with this method, better remake the function
	{

		switch (App->collision->current_collision) 
		{
		case GROUND_COLLISION:

		case UPPER_COLLISION:

		case LEFT_COLLISION:

		case RIGHT_COLLISION:

		case RIGHT_UPPER_COLLISION:

		case LEFT_UPPER_COLLISION:

		case RIGHT_GROUND_COLLISION:

		case LEFT_GROUND_COLLISION:
			break;
		}

	}
	grenade_collider_buffer = App->collision->current_collision;
}



