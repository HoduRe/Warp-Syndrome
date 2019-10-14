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

	ChangeState();	// transitions automatic states, like jumping->falling->idle
	CheckInputs();	// Checks active states (based on inputs)
	CheckColliders(); // Checks colliders
	MovePlayer();	// Moves player position
	ChangeAnimation();	// Puts the proper animation

	internal_counter++;

	return ret;
}

bool j1State::CleanUp() {

	return true;
}

void j1State::ChangeState() {
	/*if (animation_end == true) { // TODO maybe change this condition to "when reaching point A instead of when animation ends"
		switch (current_state) {
		case FREE_JUMP:
		case WALL_JUMP:
			current_state = FREE_FALLING;
			break;
		case SLIDING_TO_IDLE:
		case THROWING_GRENADE:
		case TELEPORT:
			current_state == IDLE;
			break;
		case THROWING_GRENADE_ON_AIR:
			current_state == FREE_FALLING;
			break;
		case DEAD:
			current_state == WAKE_UP;
			break;
		}
	}*/
}

void j1State::CheckInputs() {

	switch (current_state) {
	case IDLE:
	case WALK_FORWARD:
	case WALK_BACKWARD:
	case RUN_FORWARD:
	case RUN_BACKWARD:
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) { current_state = FREE_JUMP; }
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && run_counter == 6) { current_state = RUN_FORWARD; }
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && run_counter == 6) { current_state = RUN_BACKWARD; }
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT == KEY_DOWN)) {
			current_state = WALK_FORWARD;
			run_counter++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT == KEY_DOWN)) {
			current_state = WALK_BACKWARD;
			run_counter++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { current_state = THROWING_GRENADE; grenade = true; }
		else { current_state = IDLE; run_counter = 0; }
		break;
	case FREE_JUMP:
	case FREE_FALLING:
	case WALL_JUMP:
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { current_state = THROWING_GRENADE_ON_AIR; grenade = true; }
		break;
	case SLIDING_ON_RIGHT_WALL:
		if (current_state == SLIDING_ON_RIGHT_WALL && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			App->player->AddPosition(-1.0f, 1.0f);
		}
		break;
	case SLIDING_ON_LEFT_WALL:
		if (current_state == SLIDING_ON_LEFT_WALL && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			App->player->AddPosition(1.0f, 1.0f);
		}
		break;

	}

	// TODO delete the inputs that influates player move on j1Scene
	// TODO move the run_counter to player xml
	// TODO read free jump, fall and wall jump inputs
}

void j1State::CheckColliders() {

	switch (App->collision->current_collision) {
	case NONE:	// Falling from a cliff collisions
		switch (current_state) {
		case IDLE:
		case THROWING_GRENADE:
		case WALK_FORWARD:
		case RUN_FORWARD:
		case WALK_BACKWARD:
		case RUN_BACKWARD:
			current_state = FREE_FALLING;
			break;
		}
	case UPPER_COLLISION:	// Touching the roof collisions
	case GROUND_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case THROWING_GRENADE_ON_AIR:
		case WALL_JUMP:
			current_state = FREE_FALLING;
			break;
		case IDLE:
		case WALK_BACKWARD:
		case WALK_FORWARD:
		case RUN_BACKWARD:
		case RUN_FORWARD:
		case THROWING_GRENADE:
		case SLIDING_TO_IDLE:
			current_state = DEAD;
			break;
		}
		break;
	case RIGHT_COLLISION:	// Touching a right wall collisions
	case RIGHT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case THROWING_GRENADE_ON_AIR:
			current_state = SLIDING_ON_RIGHT_WALL;
			break;
		}
		break;
	case LEFT_COLLISION:	// Touching a left wall collisions
	case LEFT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case THROWING_GRENADE_ON_AIR:
			current_state = SLIDING_ON_LEFT_WALL;
			break;
		}
		break;
	case GROUND_COLLISION:	// Touching the ground collisions
		switch (current_state) {
		case FREE_FALLING:
			current_state = IDLE;
			break;
		}
		break;
	case LEFT_GROUND_COLLISION:	// Touching the ground collisions
	case RIGHT_GROUND_COLLISION:
		switch (current_state) {
		case FREE_FALLING:
		case WALK_BACKWARD:
		case WALK_FORWARD:
		case RUN_BACKWARD:
		case RUN_FORWARD:
			current_state = IDLE;
			break;
		case SLIDING_ON_LEFT_WALL:
		case SLIDING_ON_RIGHT_WALL:
			current_state = SLIDING_TO_IDLE;
			break;
		}
		break;
	}
}

void j1State::MovePlayer() {
	// X AXIS MOVEMENT
	switch (current_state) {
	case WALK_FORWARD:
	case RUN_FORWARD:
		if (current_state == WALK_FORWARD && internal_counter % 3 == 0) { App->player->AddPosition(1.0f, 0.0f); }
		else { App->player->AddPosition(1.0f, 0.0f); }	// Position iterates by 1 because of collider detection
		break;
	case WALK_BACKWARD:
	case RUN_BACKWARD:
		if (current_state == WALK_BACKWARD && internal_counter % 3 == 0) { App->player->AddPosition(-1.0f, 0.0f); }
		else { App->player->AddPosition(-1.0f, 0.0f); }	// Position iterates by 1 because of collider detection
		break;
	case FREE_JUMP:
	case FREE_FALLING:
	case WALL_JUMP:
		break;
	}

	// Y AXIS MOVEMENT
	switch (current_state) {
	case FREE_JUMP:		// TODO move this to a function
		JumpMove();
	case WALL_JUMP:
		App->player->AddPosition(0.0f, -1.0f);
		break;
	case FREE_FALLING:
		JumpMove();
		break;
	case SLIDING_ON_LEFT_WALL:
	case SLIDING_ON_RIGHT_WALL:
		App->player->AddPosition(0.0f, 1.0f);
		break;
	}		// TODO JUMPS
}

void j1State::ChangeAnimation() {

}

JumpingStates j1State::JumpMove() {

	JumpingStates ret = JST_UNKNOWN;

	if (jump_timer == 0.0f)
		jump_timer += 0.1f;
	else if (jump_timer > 0) { jump_timer += 0.1f; }

	if (jump_timer > abs(App->player->GetVelocity().y * 2.5f)) { jump_timer = abs(App->player->GetVelocity().y * 2.5f); }	// Para que no baje demasiado rápido

	if (jump_timer == abs(App->player->GetVelocity().y * 2.5f)) ret = JST_TRANSITION;
	else if (jump_timer < abs(App->player->GetVelocity().y * 2.5f)) ret = JST_GOING_UP;
	else if (jump_timer > abs(App->player->GetVelocity().y * 2.5f)) ret = JST_GOING_DOWN;


	switch (current_state)
	{
	case FREE_JUMP:
		App->player->AddPosition(0, -App->player->GetVelocity().y + jump_timer);
		break;
	case FREE_FALLING:
		App->player->AddPosition(0, -App->player->GetVelocity().y + jump_timer);

		break;
	default:
		break;
	}

	return ret;
}