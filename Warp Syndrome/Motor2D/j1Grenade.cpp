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
Grenade::Grenade() :AnimatedParticle(EntityType::E_TYPE_GRENADE)
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
	forces = { 0.0f,0.0f };
	mass = 1.0f;

	dieOnEndAnim = false;
}
//constructor
Grenade::Grenade(fPoint aPos, fPoint aSpeed, float aHealth) : AnimatedParticle("grenade", false, aPos, aSpeed, 1.0f, texture, aHealth)
{
	pos = aPos;
	speed = aSpeed;
	health = aHealth;
	fliped = false;
	texture = App->entity_m->player->texture;
	texture_section = { 0,0,0,0 };
	destroy = false;
	enabled = true;

	offset = { 0.0f,0.0f };
	forces = { 0.0f,0.0f };
	mass = 1.0f;

	App->entity_m->grenade = this;

}

// Destructor
Grenade::~Grenade()
{
	App->entity_m->grenade = nullptr;
}

// Called before the first frame
bool Grenade::Start()
{
	return true;
}

// Called each loop iteration
bool Grenade::PreUpdate()
{
	return true;
}
bool Grenade::Update(float dt) {

	bool playercantp = true;
	Integrate(dt);
	health -= dt;
	CorrectCollider(dt);

	if (App->collision->GrenadeColliderTouched()) { playercantp = false; }

	if (health <= 0.0f || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || App->input->GetMouseButtonDown(2) == KEY_DOWN)
	{//explode the granade
		destroy = true;
		App->entity_m->grenade = nullptr;
		App->entity_m->player->grenadecooldown = 2.5f;
	}
	else if ((App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN || App->input->GetMouseButtonDown(3) == KEY_DOWN) && App->entity_m->player->current_state != DYING && playercantp == true) //if the player is not dying, and the grenade hasn't been destroyed can tp
		Teleport();

	anim.StepAnimation(dt);
	return true;
}
bool Grenade::PostUpdate()
{

	App->render->Blit(texture, pos.x, pos.y, &anim.GetCurrentFrame()->animationRect);
	return true;
}

// Called before quitting
bool Grenade::CleanUp() {
	//App->tex->UnLoad(texture);//TODO don't know if its a copy of the player texture or a poniter to it, so maybe, when cleaning the player dissapears :p guess we will find out soon
	//We found out indeed
	return true;
}

void Grenade::Teleport()
{
	App->audio->PlayFx(App->scene->teleport_sfx);
	//generate a particle on the player then move the player and generate the other when TPed
	AnimatedParticle* p = new AnimatedParticle("pulsar_in", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, { 0.0f,0.0f }, { -50.0f,-43.0f });
	App->entity_m->AddEntity(p);
	App->entity_m->player->pos = pos;
	AnimatedParticle* q = new AnimatedParticle("pulsar_out", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, { 0.0f,0.0f }, { -50.0f,-43.0f });
	App->entity_m->AddEntity(q);

	destroy = true;
	App->entity_m->grenade = nullptr;
	App->entity_m->player->grenadecooldown = 2.5f;
}

void Grenade::GrenadeCollisions()
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

void Grenade::CorrectCollider(float dt) {
	fPoint measures(6, 6);
	App->collision->CheckLoop(&pos, &measures, OBJECT_GRENADE);
	switch (App->collision->current_collision) {
	case GROUND_COLLISION:
	case UPPER_COLLISION:
		speed.y = speed.y * -0.9;
		pos.y = App->collision->current_collision_buffer.collider1.y;
		break;
	case LEFT_COLLISION:
	case RIGHT_COLLISION:
		speed.x = speed.x * -0.9;
		pos.x = App->collision->current_collision_buffer.collider1.x;
		break;
	case LEFT_GROUND_COLLISION:
	case RIGHT_GROUND_COLLISION:
	case LEFT_UPPER_COLLISION:
	case RIGHT_UPPER_COLLISION:
		speed.x = speed.x * -0.9;
		speed.y = speed.y * -0.9;
		if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
			pos.y = App->collision->current_collision_buffer.collider1.y;
			pos.x = App->collision->current_collision_buffer.collider2.x;
		}
		else {
			pos.x = App->collision->current_collision_buffer.collider1.x;
			pos.y = App->collision->current_collision_buffer.collider2.y;
		}
		break;
	}
}
