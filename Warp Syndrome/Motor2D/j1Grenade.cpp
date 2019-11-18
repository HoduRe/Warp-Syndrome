#include "p2Defs.h"
#include "p2Log.h"
#include "j1State.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Grenade.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "Particles.h"
#include "j1EntityManager.h"
#include "Player.h"

j1Grenade::j1Grenade(){
}

// Destructor
j1Grenade::~j1Grenade() {}

// Called before render is available
bool j1Grenade::Awake(pugi::xml_node& ) {

	return true;
}

// Called before the first frame
bool j1Grenade::Start() {

	anim_list = &App->entity_m->player->playerAnimations;
	grenade_animation = anim_list->start->data->GetAnimFromName("grenade", anim_list);
	grenade_texture = App->entity_m->player->texture;
	grenade_vel.x = App->entity_m->player->speed.x * 3 / 2;
	grenade_vel.y = App->entity_m->player->speed.y;

	return true;
}

// Called each loop iteration
bool j1Grenade::Update(float dt) {

	if (App->state->GetGrenadeState() == true) {
		GrenadeCollisions();
		CheckMapBorder();
		GrenadeState();
		App->render->Blit(grenade_texture, grenade_position.x, grenade_position.y, &grenade_animation->data->StepAnimation()->animationRect);
	}
	else StepGrenadeCooldown();
	return true;
}

// Called before quitting
bool j1Grenade::CleanUp() {
	App->tex->UnLoad(grenade_texture);
	anim_list->clear();
	return true;
}

void j1Grenade::GrenadeCollisions() {
	App->collision->CheckLoop(&grenade_position, &grenade_measures, OBJECT_GRENADE);
	switch (App->collision->current_collision) {
	case GROUND_COLLISION:
		switch(grenade_state){
		case GST_MOVING_LEFT_DOWN:
			grenade_state = GST_MOVING_LEFT_UP;
			break;
		case GST_MOVING_RIGHT_DOWN:
			grenade_state = GST_MOVING_RIGHT_UP;
			break;
		case GST_MOVING_DOWN:
			grenade_state = GST_MOVING_UP;
			break;
		}
		break;
	case UPPER_COLLISION:
		switch (grenade_state) {
		case GST_MOVING_LEFT_UP:
			grenade_state = GST_MOVING_LEFT_DOWN;
			break;
		case GST_MOVING_RIGHT_UP:
			grenade_state = GST_MOVING_RIGHT_DOWN;
			break;
		case GST_MOVING_UP:
			grenade_state = GST_MOVING_DOWN;
			break;
		}
		break;
	case LEFT_COLLISION:
		switch (grenade_state) {
		case GST_MOVING_LEFT_DOWN:
			grenade_state = GST_MOVING_RIGHT_DOWN;
			break;
		case GST_MOVING_LEFT_UP:
			grenade_state = GST_MOVING_RIGHT_UP;
			break;
		}
		break;
	case RIGHT_COLLISION:
		switch (grenade_state) {
		case GST_MOVING_RIGHT_DOWN:
			grenade_state = GST_MOVING_LEFT_DOWN;
			break;
		case GST_MOVING_RIGHT_UP:
			grenade_state = GST_MOVING_LEFT_UP;
			break;
		}
		break;
	case RIGHT_UPPER_COLLISION:
		grenade_state = GST_MOVING_LEFT_DOWN;
		break;
	case LEFT_UPPER_COLLISION:
		grenade_state = GST_MOVING_RIGHT_DOWN;
		break;
	case RIGHT_GROUND_COLLISION:
		grenade_state = GST_MOVING_LEFT_UP;
		break;
	case LEFT_GROUND_COLLISION:
		grenade_state = GST_MOVING_RIGHT_UP;
		break;
	}
	grenade_collider_buffer = App->collision->current_collision;
}

void j1Grenade::GrenadeState() {
	
	if ((grenade_state != GST_UNKNOWN && App->collision->GrenadeColliderTouched() != true && 
		(App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN || App->input->GetMouseButtonDown(3) == KEY_DOWN))) {
		grenade_state = GST_EXPLODING;
	}
	if (grenade_state != GST_UNKNOWN && grenade_state != GST_EXPLODING) {
		grenade_time_to_explode += 0.1;
	}
	if (App->state->current_state == DYING) {
		grenade_state = GST_UNUSABLE;
	}

	switch (grenade_state) {
	case GST_UNKNOWN:
		grenade_position.x = App->entity_m->player->pos.x;
		grenade_position.y = App->entity_m->player->pos.y - App->entity_m->player->hitbox_w_h.x;
		grenade_timer.x = grenade_vel.x;
		switch (App->entity_m->player->fliped) {
		case true:
			grenade_state = GST_MOVING_LEFT_UP;
			break;
		case false:
			grenade_state = GST_MOVING_RIGHT_UP;
			break;
		}
		grenade_timer.y = grenade_vel.y;
		break;
	case GST_MOVING_UP:	// NOT BEING USED
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(0.0f, -grenade_vel.y + grenade_timer.y);
		if (grenade_timer.y >= grenade_vel.y) { grenade_state = GST_MOVING_DOWN; }
		break;
	case GST_MOVING_DOWN: // NOT BEING USED
		if(grenade_timer.y >= 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(0.0f, grenade_vel.y - grenade_timer.y);
		break;
	case GST_MOVING_LEFT_UP:
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(-grenade_vel.x, -grenade_vel.y + grenade_timer.y);
		if (grenade_timer.y >= grenade_vel.y) { grenade_state = GST_MOVING_LEFT_DOWN; }
		break;
	case GST_MOVING_LEFT_DOWN:
		if (grenade_timer.y > 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(-grenade_vel.x, -grenade_timer.y + grenade_vel.y);
		break;
	case GST_MOVING_RIGHT_UP:
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(grenade_vel.x, -grenade_vel.y + grenade_timer.y);
		if (grenade_timer.y >= grenade_vel.y) { grenade_state = GST_MOVING_RIGHT_DOWN; }
		break;
	case GST_MOVING_RIGHT_DOWN:
		if (grenade_timer.y > 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(grenade_vel.x, -grenade_timer.y + grenade_vel.y);
		break;
	case GST_EXPLODING:
		grenade_state = GST_UNKNOWN;
		grenade_timer.y = 0.0f;
		grenade_time_to_explode = 0;
		App->audio->PlayFx(App->scene->teleport_sfx);
		App->state->SetGrenadeState(false);
		fPoint position;
		fPoint measures;
		App->state->current_state = TELEPORT;
		if (App->state->current_state == TELEPORT) 
		{ 
			//generate a particle on the player then move the player and generate the other when TPed
			AnimatedParticle* p = new AnimatedParticle("pulsar_in", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, {0.0f,0.0f}, { -50.0f,-43.0f });
			App->entity_m->AddEntity(p);
			App->entity_m->player->pos= App->grenade->GetPosition();
			AnimatedParticle* q = new AnimatedParticle("pulsar_out", true, { App->entity_m->player->pos.x,App->entity_m->player->pos.y }, App->entity_m->player->texture, 200, { 0.0f,0.0f }, { -50.0f,-43.0f });
			App->entity_m->AddEntity(p);

		}
		position.x = App->entity_m->player->pos.x;
		position.y = App->entity_m->player->pos.y;
		measures.x = App->entity_m->player->pos.x;
		measures.y = App->entity_m->player->pos.y;
		App->collision->CheckLoop(&position, &measures, OBJECT_PLAYER);
		App->state->MovePlayer();
		break;
	}

	if ((grenade_time_to_explode >= 40 || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN||App->input->GetMouseButtonDown(2) == KEY_DOWN)
		|| grenade_state == GST_UNUSABLE) {
		grenade_state = GST_UNKNOWN;
		grenade_timer.y = 0.0f;
		grenade_time_to_explode = 0;
		App->state->SetGrenadeState(false);
	}

}

void j1Grenade::CheckMapBorder() {
	if (grenade_position.x <= 0 + grenade_measures.x + 1) {
		if (grenade_state == GST_MOVING_LEFT_DOWN) { grenade_state = GST_MOVING_RIGHT_DOWN; }
		else if (grenade_state == GST_MOVING_LEFT_UP) { grenade_state = GST_MOVING_RIGHT_UP; }
	}
}

void j1Grenade::AddPosition(float x, float y) {
	grenade_position.x += x;
	grenade_position.y += y;
}

bool j1Grenade::DoesGrenadeExist() {
	if (grenade_state == GST_UNKNOWN) { return false; }
	else { return true; }
}

bool j1Grenade::IsGrenadeExploding() {
	if (grenade_state == GST_EXPLODING) { return true; }
	else { return false; }
}

fPoint j1Grenade::GetPosition() {
	return grenade_position;
}

fPoint j1Grenade::GetMeasures() {
	return grenade_measures;
}

collision_type j1Grenade::ColliderBuffer() {
	return grenade_collider_buffer;
}

bool j1Grenade::GetGrenadeCooldown()
{
	if (cooldown_timer < 120)
	{
		return true;
	}
	return false;
}

void j1Grenade::StepGrenadeCooldown()
{
	if (cooldown_timer < 120)
	{
		cooldown_timer++;
	}
}

void j1Grenade::GrenadeCooldownReset()
{
	cooldown_timer = 0;
}

void j1Grenade::SetMeasures(pugi::xml_node root)
{
	grenade_measures.x= root.child("measures").attribute("w").as_uint(6);
	grenade_measures.y = root.child("measures").attribute("h").as_uint(6);

}