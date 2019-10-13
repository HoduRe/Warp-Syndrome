#include "p2Defs.h"
#include "p2Log.h"
#include "j1State.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Input.h"
#include "p2List.h"
#include "j1App.h"

j1State::j1State() : j1Module() {
	name.create("state");
}

j1State::~j1State() {}

bool j1State::Awake(pugi::xml_node& config) {
	//filename.create(config.child("load").attribute("docname").as_string());
	return true;
}

bool j1State::Start() {

	current_state = IDLE;

	return true;
}

bool j1State::Update(float dt) {

	bool ret = true;

	MoveState();	// transitions automatic states, like jumping->falling->idle
	CheckInputs();	// Checks active states (based on inputs)
	MovePlayer();	// Moves player position
	ChangeAnimation();	// Puts the proper animation

	internal_counter++;

	return ret;
}

bool j1State::CleanUp() {

	return true;
}

void j1State::MoveState() {
	if (current_state == THROWING_GRENADE_ON_AIR && internal_counter - animation_counter == 0) {	// TODO Switch 0 for the duration of the throw granade animation
		current_state == FALLING_DOWN;
	}
	else if (current_state == THROWING_GRENADE && internal_counter - animation_counter == 0) {	// TODO Switch 0 for the duration of the throw granade animation
		current_state == IDLE;
	}
	else if (current_state == DEAD && internal_counter - animation_counter == 0) {
		current_state == WAKE_UP;
	}
	else if ((current_state == JUMP_BACKWARD || current_state == WALL_JUMP_BACKWARD) &&
		internal_counter - animation_counter == 0 && (App->collision->current_collision != LEFT_COLLISION && App->collision->current_collision != LEFT_UPPER_COLLISION)) {
		current_state = FALLING_BACKWARD;
	}
	else if ((current_state == JUMP_BACKWARD || current_state == WALL_JUMP_BACKWARD) &&
		(App->collision->current_collision == LEFT_COLLISION || App->collision->current_collision == LEFT_UPPER_COLLISION)) {
		current_state = SLIDING_ON_LEFT_WALL;
	}
	else if ((current_state == JUMP_FORWARD || current_state == WALL_JUMP_FORWARD) &&
		internal_counter - animation_counter == 0 && (App->collision->current_collision != RIGHT_COLLISION && App->collision->current_collision != RIGHT_UPPER_COLLISION)) {
		current_state = FALLING_FORWARD;
	}
	else if ((current_state == JUMP_FORWARD || current_state == WALL_JUMP_FORWARD) &&
		(App->collision->current_collision == RIGHT_COLLISION || App->collision->current_collision == RIGHT_UPPER_COLLISION)) {
		current_state = SLIDING_ON_RIGHT_WALL;
	}
	else if (current_state == JUMP_UP && (App->collision->current_collision == UPPER_COLLISION || internal_counter - animation_counter == 0)) {
		current_state = FALLING_DOWN;
	}
	else if ((current_state == FALLING_BACKWARD || current_state == FALLING_FORWARD || current_state == FALLING_DOWN) &&
		(App->collision->current_collision == GROUND_COLLISION || App->collision->current_collision == LEFT_GROUND_COLLISION ||
			App->collision->current_collision == GROUND_COLLISION)) {
		current_state = IDLE;
	}
	else if ((current_state == SLIDING_ON_LEFT_WALL || current_state == SLIDING_ON_RIGHT_WALL) && (App->collision->current_collision == GROUND_COLLISION ||
		App->collision->current_collision == LEFT_GROUND_COLLISION || App->collision->current_collision == GROUND_COLLISION)) {
		current_state = SLIDING_TO_IDLE;
		animation_counter = internal_counter;
	}
	else if (current_state == SLIDING_TO_IDLE && internal_counter - animation_counter == 0) {
		current_state = IDLE;
	}
	// TODO Define an else if for state TELEPORT, when grenade is implemented
	// TODO Define an else if for state DEAD, when death is implemented
}

void j1State::CheckInputs() {
	// TODO delete the inputs that influates player move on j1Scene
	if (current_state == IDLE || current_state == WALK_FORWARD || current_state == WALK_BACKWARD
		|| current_state == RUN_FORWARD || current_state == RUN_BACKWARD) {
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			current_state = JUMP_FORWARD;
			animation_counter = internal_counter;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			current_state = JUMP_BACKWARD;
			animation_counter = internal_counter;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
			current_state = JUMP_UP;
			animation_counter = internal_counter;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && current_state == RUN_FORWARD) {
			current_state = RUN_FORWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && current_state == RUN_BACKWARD) {
			current_state = RUN_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			run_counter++;
			if (run_counter == 6) { current_state = RUN_FORWARD; run_counter = 0; }
			else { current_state = WALK_FORWARD; }	// TODO move the run_counter to player xml
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			run_counter++;
			if (run_counter == 6) { current_state = RUN_BACKWARD; run_counter = 0; }
			else { current_state = WALK_BACKWARD; }
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
			current_state = THROWING_GRENADE;
			animation_counter = internal_counter;
			granade = true;
		}
		else { current_state = IDLE; }
	}
	else if ((current_state == JUMP_BACKWARD || current_state == JUMP_FORWARD || current_state == JUMP_UP ||
		current_state == WALL_JUMP_FORWARD || current_state == WALL_JUMP_BACKWARD) && App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		current_state = THROWING_GRENADE_ON_AIR;
		animation_counter = internal_counter;
		granade = true;
	}
	else if (current_state == SLIDING_ON_RIGHT_WALL && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		current_state = WALL_JUMP_BACKWARD;
		animation_counter = internal_counter;
	}
	else if (current_state == SLIDING_ON_LEFT_WALL && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		current_state = WALL_JUMP_FORWARD;
		animation_counter = internal_counter;
	}

	return;
}

void j1State::MovePlayer() {
	// X AXIS MOVEMENT
	if (App->collision->current_collision != RIGHT_GROUND_COLLISION && (current_state == WALK_FORWARD || current_state == RUN_FORWARD)) {
		if (current_state == WALK_FORWARD && internal_counter % 3 == 0) {
			App->player->playerpos.x++;
		}
		else { App->player->playerpos.x++; }
	}
	else if (App->collision->current_collision != LEFT_GROUND_COLLISION && (current_state == WALK_BACKWARD || current_state == RUN_BACKWARD)) {
		if (current_state == WALK_FORWARD && internal_counter % 3 == 0) {
			App->player->playerpos.x--;
		}
		else { App->player->playerpos.x--; }
	}
	if ((App->collision->current_collision != RIGHT_COLLISION && App->collision->current_collision != RIGHT_UPPER_COLLISION) &&
		(current_state == JUMP_FORWARD || current_state == FALLING_FORWARD	|| current_state == WALL_JUMP_FORWARD)) {
		App->player->playerpos.x++;
	}
	else if ((App->collision->current_collision != LEFT_COLLISION && App->collision->current_collision != LEFT_UPPER_COLLISION) &&
		(current_state == JUMP_BACKWARD || current_state == FALLING_BACKWARD || current_state == WALL_JUMP_BACKWARD)) {
		App->player->playerpos.x--;
	}
	
	// Y AXIS MOVEMENT
	if ((App->collision->current_collision != RIGHT_UPPER_COLLISION && App->collision->current_collision != LEFT_UPPER_COLLISION && App->collision->current_collision != UPPER_COLLISION)
		&& (current_state == JUMP_FORWARD || current_state == WALL_JUMP_FORWARD) || current_state == JUMP_BACKWARD || current_state == WALL_JUMP_BACKWARD || current_state == JUMP_UP) {
		App->player->playerpos.y--;
	}
	else if (App->collision->current_collision != GROUND_COLLISION && (current_state == FALLING_BACKWARD
		|| current_state == FALLING_FORWARD || current_state == FALLING_DOWN)) {
		App->player->playerpos.y++;
	}

	// COLLISION MAKES A STATE SWITCH
	if ((App->collision->current_collision == RIGHT_COLLISION || App->collision->current_collision == RIGHT_UPPER_COLLISION) &&
		(current_state == JUMP_FORWARD || current_state == WALL_JUMP_FORWARD || current_state == FALLING_FORWARD)) {
		current_state = SLIDING_ON_RIGHT_WALL;
	}
	else if ((App->collision->current_collision == LEFT_COLLISION || App->collision->current_collision == LEFT_UPPER_COLLISION) &&
		(current_state == JUMP_BACKWARD || current_state == WALL_JUMP_BACKWARD || current_state == FALLING_BACKWARD)) {
		current_state = SLIDING_ON_LEFT_WALL;
	}

	if (current_state == SLIDING_ON_RIGHT_WALL || current_state == SLIDING_ON_LEFT_WALL) {
		App->player->playerpos.y++;
	}
}

void j1State::ChangeAnimation() {

}
