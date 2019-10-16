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
	wall_jump = SST_IDLE;	// Serves to reset the bool that passes from sliding to wall jumping

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
	case WALL_JUMP:
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { current_state = THROWING_GRENADE_ON_AIR; grenade = true; }
		else if (y_jumping_state == JST_GOING_DOWN) { current_state = FREE_FALLING; }
		break;
	case FREE_FALLING:
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { current_state = THROWING_GRENADE_ON_AIR; grenade = true; }
		break;
	case SLIDING_ON_RIGHT_WALL:
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			wall_jump = SST_JUMPING_LEFT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) { wall_jump = SST_FALLING_LEFT; }
		break;
	case SLIDING_ON_LEFT_WALL:
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			wall_jump = SST_JUMPING_RIGHT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) { wall_jump = SST_FALLING_RIGHT; }
		break;
	case THROWING_GRENADE:
		if (grenade == false) { current_state = IDLE; }	// TODO a function that changes this bool based on the player throwing a grenade
		break;
	case THROWING_GRENADE_ON_AIR:
		if (grenade == false) { current_state = FREE_FALLING; }	// TODO a function that changes this bool based on the player throwing a grenade
		break;
	}

	switch (current_state) {	// Just here for legibility's sake, it determines how to move on the X axis for jumps
	case FREE_JUMP:
	case WALL_JUMP:
	case FREE_FALLING:
		if (current_state != THROWING_GRENADE_ON_AIR) {	// While throwing grenade, you shouldn't move
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				x_jumping_state = JST_GOING_LEFT;
			}
			else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				x_jumping_state = JST_GOING_RIGHT;
			}
			else { x_jumping_state = JST_IDLE; }
		}
		break;
	}

	// TODO delete the inputs that influates player move on j1Scene
	// TODO move the run_counter to player xml
	// TODO read free jump, fall and wall jump inputs
	// ABOUT THE DOUBLE JUMP: happens when jumped once, doesn't happen when granade has been thrown, refreshes when collision happen
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
		case SLIDING_ON_LEFT_WALL:
		case SLIDING_ON_RIGHT_WALL:
		case TELEPORT:
		case WAKE_UP:
			current_state = FREE_FALLING;
			y_jumping_state = JST_GOING_DOWN;
			break;
		}
		break;
	case UPPER_COLLISION:	// Touching the roof collisions
	case GROUND_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case THROWING_GRENADE_ON_AIR:
		case WALL_JUMP:
			current_state = FREE_FALLING;
			y_jumping_state = JST_GOING_DOWN;
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
		case SLIDING_ON_LEFT_WALL:
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
		case SLIDING_ON_RIGHT_WALL:
		case THROWING_GRENADE_ON_AIR:
			current_state = SLIDING_ON_LEFT_WALL;
			break;
		}
		break;
	case GROUND_COLLISION:	// Touching the ground collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case THROWING_GRENADE_ON_AIR:
		case TELEPORT:
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
//			current_state = SLIDING_TO_IDLE;
			current_state = IDLE;
			break;
		}
		break;
	default:
		break;
	}
}

void j1State::MovePlayer() {
	// X AXIS MOVEMENT
	switch (current_state) {
	case WALK_FORWARD:
	case RUN_FORWARD:
		if (current_state == WALK_FORWARD && internal_counter % 3 == 0) { App->player->AddPosition(5.0f, 0.0f); }
		else { App->player->AddPosition(5.0f, 0.0f); }
		break;
	case WALK_BACKWARD:
	case RUN_BACKWARD:
		if (current_state == WALK_BACKWARD && internal_counter % 3 == 0) { App->player->AddPosition(App->player->GetVelocity().x, 0.0f); }
		else { App->player->AddPosition(-App->player->GetVelocity().x, 0.0f); }
		break;
	case FREE_JUMP:
	case FREE_FALLING:
	case WALL_JUMP:
		JumpMoveX();
		break;
	}

	// Y AXIS MOVEMENT
	JumpMoveY();
	switch (current_state) {
	case SLIDING_ON_LEFT_WALL:
	case SLIDING_ON_RIGHT_WALL:
		App->player->AddPosition(0.0f, App->player->GetVelocity().y / 2.0f);
		break;
	}		// TODO JUMPS
}

void j1State::ChangeAnimation() {

}

void j1State::JumpMoveX() {
	if (x_jumping_state == JST_GOING_LEFT) { App->player->AddPosition(-App->player->GetVelocity().x, 0.0f); }
	else if (x_jumping_state == JST_GOING_RIGHT) { App->player->AddPosition(App->player->GetVelocity().x, 0.0f); }
	else if (x_jumping_state == JST_IDLE) {}
}

void j1State::JumpMoveY() {

	if (current_state == THROWING_GRENADE_ON_AIR) { y_jumping_state = JST_TRANSITION; }

	switch (y_jumping_state) {
	case JST_UNKNOWN:
		if (current_state == FREE_JUMP || current_state == WALL_JUMP) {
			y_jumping_state = JST_GOING_UP;
			jump_timer = 0;
		}
		else if (current_state == FREE_FALLING) {
			y_jumping_state = JST_GOING_DOWN;
			jump_timer = App->player->GetVelocity().y;
		}
		break;
	case JST_GOING_UP:
		if (jump_timer >= 0 && jump_timer < App->player->GetVelocity().y) {
			jump_timer += (1.0f / 10.0f);
			App->player->AddPosition(0.0f, -App->player->GetVelocity().y + jump_timer);
		}
		else { jump_timer = App->player->GetVelocity().y; y_jumping_state = JST_GOING_DOWN; }
		break;
	case JST_TRANSITION:
		jump_timer = App->player->GetVelocity().y;
		if (current_state != THROWING_GRENADE_ON_AIR) { y_jumping_state = JST_GOING_DOWN; }
		break;
	case JST_GOING_DOWN:
		if (current_state != FREE_FALLING && current_state != FREE_JUMP) {
			y_jumping_state = JST_UNKNOWN;
			jump_timer = 0;
		}
		else if (jump_timer <= App->player->GetVelocity().y) {
			if (jump_timer > 0) { jump_timer -= (1.0f / 10.0f); }
			App->player->AddPosition(0.0f, App->player->GetVelocity().y - jump_timer);
		}
		break;
	}

	switch (App->collision->current_collision) {
	case GROUND_COLLISION:
		App->player->SetPosition(App->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		App->player->SetPosition(App->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		if (current_state == FREE_JUMP) { App->player->AddPosition(0, -1); }
		break;
	case UPPER_COLLISION:
		App->player->SetPosition(App->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		App->player->SetPosition(App->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		if (current_state == FREE_FALLING) { App->player->AddPosition(0, 1); }
		break;
	case LEFT_COLLISION:
	case RIGHT_COLLISION:
		App->player->SetPosition(App->collision->current_collision_buffer.collider1.x, App->player->GetPosition().y);
		switch (wall_jump) {
		case SST_JUMPING_RIGHT:
			App->player->AddPosition(1.0f, -1.0f);
			break;
		case SST_JUMPING_LEFT:
			App->player->AddPosition(-1.0f, -1.0f);
			break;
		case SST_FALLING_RIGHT:
			App->player->AddPosition(1.0f, 1.0f);
			break;
		case SST_FALLING_LEFT:
			App->player->AddPosition(-1.0f, 1.0f);
			break;
		default:
			break;
		}
		break;
	case LEFT_GROUND_COLLISION:
	case RIGHT_GROUND_COLLISION:
		if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
			App->player->SetPosition(App->collision->current_collision_buffer.collider2.x, App->collision->current_collision_buffer.collider1.y);
		}
		else if (App->collision->current_collision_buffer.is_first_collider_horizontal == false) {
			App->player->SetPosition(App->collision->current_collision_buffer.collider1.x, App->collision->current_collision_buffer.collider2.y);
		}
		if (App->collision->current_collision == LEFT_GROUND_COLLISION) { App->player->AddPosition(1, 0); }
		else { App->player->AddPosition(-1, 0); }
		break;
	case LEFT_UPPER_COLLISION:
	case RIGHT_UPPER_COLLISION:
		if(App->collision->current_collision_buffer.is_first_collider_horizontal == true){
			App->player->SetPosition(App->collision->current_collision_buffer.collider2.x, App->collision->current_collision_buffer.collider1.y);
			}
		else if(App->collision->current_collision_buffer.is_first_collider_horizontal == false){
			App->player->SetPosition(App->collision->current_collision_buffer.collider1.x, App->collision->current_collision_buffer.collider2.y);
			}
		switch (wall_jump) {
		case SST_JUMPING_RIGHT:
			App->player->AddPosition(1.0f, -1.0f);
			break;
		case SST_JUMPING_LEFT:
			App->player->AddPosition(-1.0f, -1.0f);
			break;
		case SST_FALLING_RIGHT:
			App->player->AddPosition(1.0f, 1.0f);
			break;
		case SST_FALLING_LEFT:
			App->player->AddPosition(-1.0f, 1.0f);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/*if (animation_end == true) {
	case SLIDING_TO_IDLE:
	case TELEPORT:
		current_state == IDLE;
		break;
	case DEAD:
		current_state == WAKE_UP;
		break;
	}
}*/
