#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1State.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Grenade.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "p2List.h"
#include "Animations.h"
#include "j1App.h"
#include "level_manager.h"
#include "j1EntityManager.h"
#include "Player.h"

j1State::j1State() : j1Module() {
	name.create("state");
}

j1State::~j1State() {}

bool j1State::Awake(pugi::xml_node& config) {
	return true;
}

bool j1State::Start() {

	current_state = IDLE;
	SetGrenadeState(false);
	run_counter = 0;
	jump_timer = 0;
	wall_jump_timer = App->entity_m->player->speed.x;
	wall_jump = SST_IDLE;
	x_jumping_state = JST_IDLE;
	y_jumping_state = JST_UNKNOWN;
	return true;
}

bool j1State::Update(float dt) {

	bool ret = true;
	fPoint playerposbuffer = App->entity_m->player->GetPosition();
	bufferlaststate = current_state;
	wall_jump = SST_IDLE;	// Serves to reset the bool that passes from sliding to wall jumping

	if (god_mode == false) {
		StepCurrentAnimation();	// steps the current animation
		CheckInputs();	// Checks active states (based on inputs)
		CheckCollisions(); // Checks colliders
		MovePlayer();	// Moves player position
		App->entity_m->player->fliped=(FlipPlayer(App->entity_m->player->GetPosition(), playerposbuffer));//flips the player
		CheckAnimation(current_state, bufferlaststate);	
		

	}
	else { GodMode(); }	// moves the player in God Mode

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
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			current_state = FREE_JUMP;
			App->audio->PlayFx(App->scene->jump_sfx);
		}
		else if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_DOWN) && App->grenade->DoesGrenadeExist() == false && !App->grenade->GetGrenadeCooldown()) {
			current_state = THROWING_GRENADE;
			SetGrenadeState(true);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && run_counter == 20) { current_state = RUN_FORWARD;  }
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && run_counter == 20) { current_state = RUN_BACKWARD; }
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D == KEY_DOWN)) {
			current_state = WALK_FORWARD; 
			run_counter++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A == KEY_DOWN)) {
			current_state = WALK_BACKWARD;
			run_counter++;
		}
		else { current_state = IDLE; run_counter = 0; }
		break;
	case FREE_JUMP:
	case WALL_JUMP:
		if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_DOWN) && App->grenade->DoesGrenadeExist() == false &&!App->grenade->GetGrenadeCooldown()) {
		current_state = THROWING_GRENADE_ON_AIR;
		SetGrenadeState(true);
		}
		else if (y_jumping_state == JST_GOING_DOWN) { current_state = FREE_FALLING; }
		break;
	case FREE_FALLING:
		if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_DOWN) && App->grenade->DoesGrenadeExist() == false && !App->grenade->GetGrenadeCooldown()) {
			current_state = THROWING_GRENADE_ON_AIR;
			SetGrenadeState(true);
		}
		break;
	case SLIDING_ON_RIGHT_WALL:
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			y_jumping_state = JST_GOING_UP;
			wall_jump = SST_JUMPING_LEFT;
			wall_jump_extra_move = SST_JUMPING_LEFT;
			App->audio->PlayFx(App->scene->jump_sfx);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) { wall_jump = SST_FALLING_LEFT; }
		break;
	case SLIDING_ON_LEFT_WALL:
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_state = WALL_JUMP;
			y_jumping_state = JST_GOING_UP;
			wall_jump = SST_JUMPING_RIGHT;
			wall_jump_extra_move = SST_JUMPING_RIGHT;
			App->audio->PlayFx(App->scene->jump_sfx);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) { wall_jump = SST_FALLING_RIGHT; }
		break;
	case THROWING_GRENADE:
		if (App->grenade->DoesGrenadeExist() == true) { current_state = IDLE; }
		break;
	case THROWING_GRENADE_ON_AIR:
		if (App->grenade->DoesGrenadeExist() == true) { current_state = FREE_FALLING; y_jumping_state = JST_GOING_DOWN; }
		break;
	}

	switch (current_state) {	// Just here for legibility's sake, it determines how to move on the X axis for jumps
	case WALL_JUMP:
	case FREE_JUMP:
	case FREE_FALLING:
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			x_jumping_state = JST_GOING_LEFT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			x_jumping_state = JST_GOING_RIGHT;
		}
		else { x_jumping_state = JST_IDLE; }
		break;
	default:
		break;
	}

	// TODO move the run_counter to player xml
	// ABOUT THE DOUBLE JUMP: happens when jumped once, doesn't happen when granade has been thrown, refreshes when collision happen
}

void j1State::CheckCollisions() {

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
		case TELEPORT:
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
			current_state = DYING;
			y_jumping_state = JST_UNKNOWN;
			break;
		}
		break;
	case RIGHT_COLLISION:	// Touching a right wall collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_LEFT_WALL:
		case THROWING_GRENADE_ON_AIR:
		case TELEPORT:
			current_state = SLIDING_ON_RIGHT_WALL;
			y_jumping_state = JST_UNKNOWN;
			break;
		}
		break;
	case RIGHT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_LEFT_WALL:
		case THROWING_GRENADE_ON_AIR:
		case TELEPORT:
			current_state = SLIDING_ON_RIGHT_WALL;
			y_jumping_state = JST_UNKNOWN;
			break;
		}
		break;
	case LEFT_COLLISION:	// Touching a left wall collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_RIGHT_WALL:
		case THROWING_GRENADE_ON_AIR:
		case TELEPORT:
			current_state = SLIDING_ON_LEFT_WALL;
			y_jumping_state = JST_UNKNOWN;
			break;
		}
		break;
	case LEFT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_RIGHT_WALL:
		case THROWING_GRENADE_ON_AIR:
		case TELEPORT:
			current_state = SLIDING_ON_LEFT_WALL;
			y_jumping_state = JST_UNKNOWN;
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
			y_jumping_state = JST_UNKNOWN;
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
		case TELEPORT:
		case SLIDING_ON_LEFT_WALL:
		case SLIDING_ON_RIGHT_WALL:
			current_state = IDLE;
			y_jumping_state = JST_UNKNOWN;
			break;
		}
		break;
	default:
		break;
	}
	switch (App->collision->current_collision) {	// Avoids horizontal vibration
	case LEFT_GROUND_COLLISION:
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) { current_state = WALK_FORWARD; }
		break;
	case RIGHT_GROUND_COLLISION:
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) { current_state = WALK_BACKWARD; }
		break;
	}
	if (current_state!=DYING && (App->collision->DeathColliderTouched() == true || App->entity_m->player->GetPosition().y - App->entity_m->player->GetWidthHeight().y > App->map->data.height * App->map->data.tile_height)) {
		current_state = DYING;
		App->audio->PlayFx(App->scene->death_sfx, 0);
	}
	if (current_state == DYING) {
		death_counter++;
		if (App->entity_m->player->currentAnim->data->GetAnimationFinish()) { // TODO put this into an xml? It's the length of the dying animation
			App->level_m->RestartLevel();
		}
	}
	if (App->collision->DoorColliderTouched() == true) { App->level_m->ChangeToNextLevel(); }
}

void j1State::MovePlayer() {

	// X AXIS MOVEMENT
	switch (current_state) {
	case WALK_FORWARD:
	case RUN_FORWARD:
		if (current_state == WALK_FORWARD) { App->entity_m->player->AddPosition(App->entity_m->player->speed.x / 2, 0.0f); }
		else { App->entity_m->player->AddPosition(App->entity_m->player->speed.x, 0.0f); }
		break;
	case WALK_BACKWARD:
	case RUN_BACKWARD:
		if (current_state == WALK_BACKWARD) { App->entity_m->player->AddPosition(-App->entity_m->player->speed.x / 2, 0.0f); }
		else { App->entity_m->player->AddPosition(-App->entity_m->player->speed.x, 0.0f); }
		break;
	case FREE_JUMP:
	case FREE_FALLING:
	case WALL_JUMP:
		JumpMoveX();
		break;
	}

	// Y AXIS MOVEMENT
	if (current_state != DYING) {
		JumpMoveY();
		switch (current_state) {
		case SLIDING_ON_LEFT_WALL:
		case SLIDING_ON_RIGHT_WALL:
			App->entity_m->player->AddPosition(0.0f, App->entity_m->player->speed.y / 2.0f);
			break;
		}

		CheckMapBorder();
		AvoidShaking();
	}
}

void j1State::JumpMoveX() {
	switch (App->collision->current_collision) {
	case LEFT_COLLISION:
	case LEFT_GROUND_COLLISION:
	case LEFT_UPPER_COLLISION:
	case RIGHT_COLLISION:
	case RIGHT_GROUND_COLLISION:
	case RIGHT_UPPER_COLLISION:
		break;
	default:
		switch (current_state) {
		case WALL_JUMP:
			if (x_jumping_state == JST_GOING_LEFT && wall_jump_timer < App->entity_m->player->speed.x * 7 / 8) {
				App->entity_m->player->AddPosition(-App->entity_m->player->speed.x, 0.0f);
			}
			else if (x_jumping_state == JST_GOING_RIGHT && wall_jump_timer < App->entity_m->player->speed.x * 7 / 8) {
				App->entity_m->player->AddPosition(App->entity_m->player->speed.x, 0.0f);
			}
			else if (x_jumping_state == JST_IDLE) {}
			if (wall_jump_extra_move == SST_JUMPING_LEFT && wall_jump_timer > 0) {
				App->entity_m->player->AddPosition(-wall_jump_timer, 0);
				wall_jump_timer -= 0.1;
			}
			else if (wall_jump_extra_move == SST_JUMPING_RIGHT && wall_jump_timer > 0) {
				App->entity_m->player->AddPosition(wall_jump_timer, 0);
				wall_jump_timer -= 0.1;
			}
			break;
		default:
			if (x_jumping_state == JST_GOING_LEFT) { App->entity_m->player->AddPosition(-App->entity_m->player->speed.x, 0.0f); }
			else if (x_jumping_state == JST_GOING_RIGHT) { App->entity_m->player->AddPosition(App->entity_m->player->speed.x, 0.0f); }
			else if (x_jumping_state == JST_IDLE) {}
			break;
		}
		break;
	}
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
			jump_timer = App->entity_m->player->speed.y;
		}
		break;
	case JST_GOING_UP:
		if (jump_timer >= 0 && jump_timer < App->entity_m->player->speed.y) {
			jump_timer += (1.0f / 10.0f);
			App->entity_m->player->AddPosition(0.0f, -App->entity_m->player->speed.y + jump_timer);
		}
		else { jump_timer = App->entity_m->player->speed.y; y_jumping_state = JST_GOING_DOWN; }
		break;
	case JST_TRANSITION:
		jump_timer = App->entity_m->player->speed.y;
		if (current_state != THROWING_GRENADE_ON_AIR) { y_jumping_state = JST_GOING_DOWN; }
		break;
	case JST_GOING_DOWN:
		if (current_state != FREE_FALLING && current_state != FREE_JUMP) {
			y_jumping_state = JST_UNKNOWN;
			jump_timer = 0;
		}
		else if (jump_timer <= App->entity_m->player->speed.y*100) {
			if (jump_timer > 0) { jump_timer -= (1.0f / 10.0f); }
			App->entity_m->player->AddPosition(0.0f, App->entity_m->player->speed.y - jump_timer);
		}
		break;
	}
}

void j1State::AvoidShaking() {

	// This switch prevents shaking when colliding happens
	switch (App->collision->current_collision) {
	case GROUND_COLLISION:
		App->entity_m->player->SetPosition(App->entity_m->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		if (current_state == FREE_JUMP) { App->entity_m->player->AddPosition(0, -1); }
		break;
	case UPPER_COLLISION:
		App->entity_m->player->SetPosition(App->entity_m->player->GetPosition().x, App->collision->current_collision_buffer.collider1.y);
		if (current_state == FREE_FALLING) { App->entity_m->player->AddPosition(0, 1); }
		break;
	case LEFT_COLLISION:
	case RIGHT_COLLISION:
		App->entity_m->player->SetPosition(App->collision->current_collision_buffer.collider1.x + (App->entity_m->player->GetWidthHeight().x) / 2, App->entity_m->player->GetPosition().y);
		switch (wall_jump) {
		case SST_JUMPING_RIGHT:
			App->entity_m->player->AddPosition(1.0f, -1.0f);
			current_state = WALL_JUMP;
			break;
		case SST_JUMPING_LEFT:
			App->entity_m->player->AddPosition(-1.0f, -1.0f);
			current_state = WALL_JUMP;
			break;
		case SST_FALLING_RIGHT:
			App->entity_m->player->AddPosition(1.0f, 1.0f);
			break;
		case SST_FALLING_LEFT:
			App->entity_m->player->AddPosition(-1.0f, 1.0f);
			break;
		default:
			break;
		}
		break;
	case LEFT_GROUND_COLLISION:
	case RIGHT_GROUND_COLLISION:
		if (current_state != WALK_BACKWARD && current_state != WALK_FORWARD && current_state != FREE_JUMP) {
			if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
				App->entity_m->player->SetPosition(App->collision->current_collision_buffer.collider2.x + (App->entity_m->player->GetWidthHeight().x / 2), App->collision->current_collision_buffer.collider1.y);
			}
			else if (App->collision->current_collision_buffer.is_first_collider_horizontal == false) {
				App->entity_m->player->SetPosition(App->collision->current_collision_buffer.collider1.x + (App->entity_m->player->GetWidthHeight().x / 2), App->collision->current_collision_buffer.collider2.y);
			}
		}
		break;
	case LEFT_UPPER_COLLISION:
	case RIGHT_UPPER_COLLISION:
		if (current_state != SLIDING_ON_LEFT_WALL && current_state != SLIDING_ON_RIGHT_WALL) {
			if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
				App->entity_m->player->SetPosition(App->collision->current_collision_buffer.collider2.x + (App->entity_m->player->GetWidthHeight().x / 2), App->collision->current_collision_buffer.collider1.y);
			}
			else if (App->collision->current_collision_buffer.is_first_collider_horizontal == false) {
				App->entity_m->player->SetPosition(App->collision->current_collision_buffer.collider1.x + (App->entity_m->player->GetWidthHeight().x / 2), App->collision->current_collision_buffer.collider2.y);
			}
		}
		switch (wall_jump) {
		case SST_FALLING_RIGHT:
			App->entity_m->player->AddPosition(1.0f, 1.0f);
			break;
		case SST_FALLING_LEFT:
			App->entity_m->player->AddPosition(-1.0f, 1.0f);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (current_state != WALL_JUMP) {
		wall_jump_timer = App->entity_m->player->speed.x;
		if (current_state != WALL_JUMP) { wall_jump_extra_move = SST_IDLE; }
	}
}

void j1State::CheckMapBorder() {
	if (App->entity_m->player->GetPosition().x <= 5) {
		switch (App->collision->current_collision) {
		case GROUND_COLLISION:
		case LEFT_GROUND_COLLISION:
		case RIGHT_GROUND_COLLISION:
			current_state = IDLE;
			App->entity_m->player->SetPosition(5, App->entity_m->player->GetPosition().y);
			break;
		default:
			App->entity_m->player->SetPosition(App->entity_m->player->speed.x, App->entity_m->player->GetPosition().y);
			x_jumping_state = JST_IDLE;
			break;
		}
	}
}

bool j1State::FlipPlayer(fPoint currentpos, fPoint lastpos)
{
	bool fliped = App->entity_m->player->fliped;
	if (currentpos.x < lastpos.x)fliped = true;
	else if (currentpos.x > lastpos.x)fliped = false;
	
	
	if (App->collision->LeftCollision())fliped = true;
	if (App->collision->RightCollision())fliped = false;

	return fliped;
}

void j1State::ChangeAnimation(Animation_list animations)
{
	p2List<Animations*>* pAnimList = App->entity_m->player->GetAnimationList();//pointer to the player's animation list
	p2List_item<Animations*>* currentanim = App->entity_m->player->currentAnim;//pointer to the current animation

	currentanim->data->ResetAnimation();//resets the current animation before changing to another one

	p2List_item<Animations*>* newanim = nullptr;
	switch (animations)
	{
	case AL_IDLE:
		newanim = currentanim->data->GetAnimFromName("idle", pAnimList);
		break;
	case AL_CROUCH:
		newanim = currentanim->data->GetAnimFromName("crouch", pAnimList);
		break;
	case AL_RUNNING:
		newanim = currentanim->data->GetAnimFromName("running", pAnimList);
		break;
	case AL_CROUCHING:
		newanim = currentanim->data->GetAnimFromName("crouching", pAnimList);
		break;
	case AL_THROW_GRANADE:
		newanim = currentanim->data->GetAnimFromName("throw_granade", pAnimList);
		break;
	case AL_FORWARD_JUMP:
		newanim = currentanim->data->GetAnimFromName("forward_jump", pAnimList);
		break;
	case AL_FALLING:
		newanim = currentanim->data->GetAnimFromName("falling", pAnimList);
		break;
	case AL_SLIDING:
		newanim = currentanim->data->GetAnimFromName("sliding", pAnimList);
		break;
	case AL_HURT:
		newanim = currentanim->data->GetAnimFromName("hurt", pAnimList);
		break;
	case AL_WALLSLIDING:
		newanim = currentanim->data->GetAnimFromName("wallsliding", pAnimList);
		break;
	case AL_WALLSLIDING_TO_IDLE:
		newanim = currentanim->data->GetAnimFromName("wallsliding_to_idle", pAnimList);
		break;
	case AL_WALLJUMPING:
		newanim = currentanim->data->GetAnimFromName("walljump", pAnimList);
		break;
	case AL_WAKEUP:
		newanim = currentanim->data->GetAnimFromName("wakeup", pAnimList);
		break;
	case AL_WALKING:
		newanim = currentanim->data->GetAnimFromName("walking", pAnimList);
		break;
	case AL_DYING:
		newanim = currentanim->data->GetAnimFromName("hurt", pAnimList);
		break;
	case AL_UNKNOWN:
		newanim = currentanim->data->GetAnimFromName("", pAnimList);
		break;
	default:
		break;
	}

	App->entity_m->player->currentAnim=newanim;//sets the current animation of the player

}

Animation_state j1State::StepCurrentAnimation()
{
	Animation_state state = AS_UNKNOWN;

	p2List_item<Animations*>* currentanim = App->entity_m->player->currentAnim;
	FrameInfo* frame;
	frame = currentanim->data->StepAnimation();

	App->entity_m->player->currentframe=frame;

	return state;
}

void j1State::CheckAnimation(state_list currentstate, state_list laststate)
{
	if (currentstate != laststate)
	{
		switch (currentstate)
		{
		case NONE:
			break;
		case IDLE:
			ChangeAnimation(AL_IDLE);
			break;
		case WALK_FORWARD:
		case WALK_BACKWARD:
			ChangeAnimation(AL_WALKING);
			break;
		case RUN_FORWARD:
		case RUN_BACKWARD:
			ChangeAnimation(AL_RUNNING);
			break;
		case FREE_JUMP:
			ChangeAnimation(AL_FORWARD_JUMP);
			break;
		case FREE_FALLING:
			ChangeAnimation(AL_FALLING);
			break;
		case THROWING_GRENADE:
		case THROWING_GRENADE_ON_AIR:
			ChangeAnimation(AL_THROW_GRANADE);
			break;
		case TELEPORT:
			//NEEEDS ANIMATION
			break;
		case SLIDING_ON_RIGHT_WALL:
		case SLIDING_ON_LEFT_WALL:
			ChangeAnimation(AL_WALLSLIDING);
			break;
		case WALL_JUMP:
			ChangeAnimation(AL_WALLJUMPING);
			break;
		case SLIDING_TO_IDLE:
			ChangeAnimation(AL_WALLSLIDING_TO_IDLE);
			break;
		case WAKE_UP:
			ChangeAnimation(AL_WAKEUP);
			break;
		case DYING:
			ChangeAnimation(AL_DYING);
			break;
		case GOD_MODE:
			ChangeAnimation(AL_IDLE);
			break;
		default:
			break;
		}
	}
}

bool j1State::GetGrenadeState()
{
	return grenade;
}

void j1State::SetGrenadeState(bool state)
{
	grenade = state;
	if (state == false)App->grenade->GrenadeCooldownReset();

}

bool j1State::BlitColliders() {
	if (blit_colliders == true) { return true; }
	else { return false; }
}

void j1State::SetBlitColliders() {
	blit_colliders = !blit_colliders;
}

bool j1State::GetGodmode() {
	if (god_mode == true) { return true; }
	else { return false; }
}

void j1State::SetGodmode() {
	god_mode = !god_mode;
}

void j1State::SetGodmode(bool state) {
	if (state == true) { god_mode = true; }
	else if (state == false) { god_mode = false; }
}

void j1State::GodMode() {
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		App->entity_m->player->AddPosition(-App->entity_m->player->speed.x * 4, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		App->entity_m->player->AddPosition(App->entity_m->player->speed.x * 4, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		App->entity_m->player->AddPosition(0, -App->entity_m->player->speed.x * 4);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		App->entity_m->player->AddPosition(0, App->entity_m->player->speed.x * 4);
	}

}
