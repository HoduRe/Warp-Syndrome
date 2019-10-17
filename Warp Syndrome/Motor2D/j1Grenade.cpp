#include "p2Defs.h"
#include "p2Log.h"
#include "j1State.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Grenade.h"
#include "j1Render.h"
#include "j1App.h"

j1Grenade::j1Grenade() {
	name.create("grenade");
}

// Destructor
j1Grenade::~j1Grenade() {}

// Called before render is available
bool j1Grenade::Awake(pugi::xml_node&) {
	grenade_measures.x = 6;
	grenade_measures.y = 6;
	return true;
}

// Called before the first frame
bool j1Grenade::Start() {

	return true;
}

// Called each loop iteration
bool j1Grenade::Update(float dt) {

	if (App->state->grenade == true) {
		GrenadeCollisions();
		GrenadeState();
//		App->render->Blit(); DO A FUNCTION? or just put it in an if
	}

	return true;
}

// Called before quitting
bool j1Grenade::CleanUp() {

	return true;
}

void j1Grenade::GrenadeCollisions() {
	App->collision->CheckLoop(&grenade_position, &grenade_measures);
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
}

void j1Grenade::GrenadeState() {
	switch (grenade_state) {
	case GST_UNKNOWN:
		grenade_position = App->player->GetPosition();
		grenade_timer.x = App->player->GetVelocity().x;
		switch (App->state->current_state) {
		case THROWING_GRENADE:
		case THROWING_GRENADE_ON_AIR:
			switch (App->state->grenade_direction) {
			case JST_GOING_LEFT:
				grenade_state = GST_MOVING_LEFT_UP;
				grenade_timer.y = App->player->GetVelocity().y;
				break;
			case JST_GOING_RIGHT:
				grenade_state = GST_MOVING_RIGHT_UP;
				grenade_timer.y = App->player->GetVelocity().y;
				break;
			}
			break;
		}
		break;
	case GST_MOVING_UP:
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(0.0f, -App->player->GetVelocity().y + grenade_timer.y);
		if (grenade_timer.y >= App->player->GetVelocity().y) { grenade_state = GST_MOVING_DOWN; }
		break;
	case GST_MOVING_DOWN:
		if(grenade_timer.y >= 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(0.0f, App->player->GetVelocity().y - grenade_timer.y);
		break;
	case GST_MOVING_LEFT_UP:
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(-App->player->GetVelocity().x, -App->player->GetVelocity().y + grenade_timer.y);
		if (grenade_timer.y >= App->player->GetVelocity().y) { grenade_state = GST_MOVING_LEFT_DOWN; }
		break;
	case GST_MOVING_LEFT_DOWN:
		if (grenade_timer.y > 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(-App->player->GetVelocity().x, -grenade_timer.y + App->player->GetVelocity().y);
		break;
	case GST_MOVING_RIGHT_UP:
		grenade_timer.y += (1.0f / 10.0f);
		AddPosition(App->player->GetVelocity().x, -App->player->GetVelocity().y + grenade_timer.y);
		if (grenade_timer.y >= App->player->GetVelocity().y) { grenade_state = GST_MOVING_RIGHT_DOWN; }
		break;
	case GST_MOVING_RIGHT_DOWN:
		if (grenade_timer.y > 0) { grenade_timer.y -= (1.0f / 10.0f); }
		AddPosition(App->player->GetVelocity().x, -grenade_timer.y + App->player->GetVelocity().y);
		break;
	case GST_EXPLODING:
		grenade_state = GST_UNKNOWN;
		grenade_position.x = 0.0f;
		grenade_position.y = 0.0f;
		grenade_timer.x = 0.0f;
		grenade_timer.y = 0.0f;
		App->state->grenade = false;
		break;
	}
}

void j1Grenade::AddPosition(float x, float y) {
	grenade_position.x += x;
	grenade_position.y += y;
}