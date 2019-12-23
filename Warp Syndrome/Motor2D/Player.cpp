#include "p2Defs.h"
#include "p2Log.h"
#include "p2SString.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Grenade.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "level_manager.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "Player.h"
#include "Entity.h"

#include "SDL/include/SDL.h"


Player::Player() :Character(E_TYPE_PLAYER)
{
	currentframe = nullptr;
	currentAnim = nullptr;
	hitbox_w_h = { 0,0 };
	texture = nullptr;
	//creates the name of the class for usage to target the right node of coniguration in the awake function
	//name.create("player");
}

Player::~Player()
{
	this->CleanUp();
}



bool Player::Awake(pugi::xml_node& config)
{
	//gets the file name of the player document from the config document
	filename.create(config.child("load").attribute("docname").as_string());
	return true;
};


bool Player::Start()
{

	bool ret = true;
	fliped = false;
	//loads the player document
	pugi::xml_parse_result result = playerdoc.load_file(filename.GetString());

	if (result == NULL)
	{
		LOG("Could not load player document. pugi error: %s", result.description());
		ret = false;
	}

	//initializes playernode to be the root node of the doc

	playernode = playerdoc.child("player");
	LoadAnimations(playernode);

	pugi::xml_node texturenode = playernode.child("texture");

	ResetPlayerToStart();
	speed.x = playernode.child("velocity").attribute("x").as_float();
	speed.y = playernode.child("velocity").attribute("y").as_float();
	hitbox_w_h.x = playernode.child("hitbox").attribute("w").as_int();
	hitbox_w_h.y = playernode.child("hitbox").attribute("h").as_int();

	//Load image
	p2SString s = texturenode.child("folder").text().as_string();
	p2SString q = texturenode.child("load").attribute("texturename").as_string();

	texture = App->tex->Load(PATH(texturenode.child("folder").text().as_string(), texturenode.child("load").attribute("texturename").as_string()));

	p2List_item<Animations*>* defaultanim = animations_list.start->data->GetAnimFromName("idle", &animations_list);
	currentAnim = defaultanim;

	current_state = IDLE;
	run_counter = 0.0f;
	jump_timer = 0.0f;
	wall_jump_timer = 0.0f;
	wall_jump = SST_IDLE;
	x_jumping_state = JST_IDLE;
	y_jumping_state = JST_UNKNOWN_Y;
	playerdoc.reset();
	return ret;
}

bool Player::PreUpdate()
{

	//Logic to spawn the grenade
	if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_DOWN))
		throwinggrenade = true;

	if (throwinggrenade)
	{
		if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT || App->input->GetMouseButtonDown(1) == KEY_REPEAT))
			App->input->GetMousePosition(throwgrenade_vec.x, throwgrenade_vec.y);//gets the vector to throw the grenade to

		//spawns the grenade

		if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_UP || App->input->GetMouseButtonDown(1) == KEY_UP) &&//if button pressed
			App->entity_m->grenade == nullptr &&//and there is no current grenade 
			grenadecooldown <= 0.0f//and the cooldown has finished
			)//throw grenade
		{
			fPoint grenade_pos;//sets the new grenade position to the center of the player
			grenade_pos.x = pos.x;
			grenade_pos.y = pos.y - (hitbox_w_h.y / 2);

			throwgrenade_vec.x -= App->render->camera.x;
			throwgrenade_vec.y -= App->render->camera.y;

			fPoint grenadedirection;//vector direction of the grenade
			grenadedirection.x = throwgrenade_vec.x - grenade_pos.x;
			grenadedirection.y = throwgrenade_vec.y - grenade_pos.y;

			float vec_module = sqrtf((grenadedirection.x * grenadedirection.x) + (grenadedirection.y * grenadedirection.y));//module of the grenade direction module
			grenadedirection.x /= vec_module;
			grenadedirection.y /= vec_module;
			grenadedirection.x *= 500;
			grenadedirection.y *= 500;
			grenadecooldown = 2.5f;
			Grenade* newgrenade = new Grenade(grenade_pos, grenadedirection, 5.0f);
			App->entity_m->AddEntity(newgrenade);
			throwinggrenade = false;
		}
	}

	return true;
}
bool Player::Update(float dt)
{
	if (grenadecooldown > 0.0f)
		grenadecooldown -= dt;

	fPoint playerposbuffer = pos;
	bufferlaststate = current_state;
	wall_jump = SST_IDLE;	// Serves to reset the bool that passes from sliding to wall jumping

	if (god_mode == false) {
		StepCurrentAnimation(dt);	// steps the current animation
		CheckInputs(dt);	// Checks active states (based on inputs)
		CheckCollisions(); // Checks colliders
		MovePlayer(dt);	// Moves player position
		fliped = (FlipCharacter(pos, playerposbuffer));//flips the player
		CheckAnimation(current_state, bufferlaststate);
	}
	else { GodMode(dt); }	// moves the player in God Mode

	return true;
}

bool Player::PostUpdate()
{
	if (currentframe != NULL)
		App->render->Blit(texture, pos.x, pos.y - currentframe->animationRect.h - currentframe->textureoffset.y, &currentframe->animationRect, fliped, currentframe->textureoffset.x);
	if (throwinggrenade && App->dt!=0.0f)
	{
		int x1 = pos.x;
		int y1 = pos.y - (hitbox_w_h.y / 2);
		int x2 = 0;
		int y2 = 0;
		App->input->GetMousePosition(x2, y2);
		x2 -= App->render->camera.x;
		y2 -= App->render->camera.y;

		if (App->entity_m->grenade!=nullptr||grenadecooldown>0.0f)
			App->render->DrawLine(x1, y1, x2, y2, 255, 0, 0, 255);
		else App->render->DrawLine(x1, y1, x2, y2, 0, 255, 0, 255);
	}

	return true;
}

// Called before quitting
bool Player::CleanUp()
{
	playerdoc.reset();
	if (texture != nullptr)
		App->tex->UnLoad(texture);
	return true;
}

// Load / Save properties
bool Player::Load(pugi::xml_node& data)
{
	coins = data.attribute("coins").as_int(0);
	return true;
}
bool Player::Save(pugi::xml_node& data) const
{

	data.append_attribute("state") = int(current_state);
	data.append_attribute("coins") = coins;
	return true;
}

//Loads all the animations needed for the player
//returns false if no animation has been loaded, otherwise returns true

bool Player::ResetPlayerToStart() {
	p2List_item<Object*>* startingpoint = nullptr;
	//search for the player spawn point
	bool found = false;
	//for every group
	p2List_item<ObjectGroup*>* group = App->map->data.objgroups.start;
	while (group != NULL && !found) {
		//for every item in this group
		p2List_item<Object*>* object = group->data->objlist.start;
		while (object != NULL && !found) {
			if (object->data->type == 4) {
				startingpoint = object;
				found = true;
			} object = object->next;
		} group = group->next;
	}

	if (startingpoint != NULL) {
		pos.x = startingpoint->data->boundingbox.x;
		pos.y = startingpoint->data->boundingbox.y;
	}
	else { //default in case nothing loads
		pos.x = 0.0f;
		pos.y = 0.0f;
	}

	current_state = IDLE;

	return true;
}

void Player::CheckInputs(float dt) {

	run_counter;
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
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && run_counter >= 0.333f) { current_state = RUN_FORWARD; }
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && run_counter >= 0.333f) { current_state = RUN_BACKWARD; }
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D == KEY_DOWN)) {
			current_state = WALK_FORWARD;
			run_counter += dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A == KEY_DOWN)) {
			current_state = WALK_BACKWARD;
			run_counter += dt;
		}
		else { current_state = IDLE; run_counter = 0.0f; }
		break;
	case FREE_JUMP:
	case WALL_JUMP:
		if (y_jumping_state == JST_GOING_DOWN) { current_state = FREE_FALLING; }
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
	// ABOUT THE DOUBLE JUMP: happens when jumped once, doesn't happen when granade has been thrown, refreshes when collision happen WIP
}

void Player::CheckCollisions() {

	switch (App->collision->current_collision) {
	case NONE:	// Falling from a cliff collisions
		switch (current_state) {
		case IDLE:
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
		case SLIDING_TO_IDLE:
			current_state = DYING;
			y_jumping_state = JST_UNKNOWN_Y;
			break;
		}
		break;
	case RIGHT_COLLISION:	// Touching a right wall collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_LEFT_WALL:
		case TELEPORT:
			current_state = SLIDING_ON_RIGHT_WALL;
			y_jumping_state = JST_UNKNOWN_Y;
			break;
		}
		break;
	case RIGHT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_LEFT_WALL:
		case TELEPORT:
			current_state = SLIDING_ON_RIGHT_WALL;
			y_jumping_state = JST_UNKNOWN_Y;
			break;
		}
		break;
	case LEFT_COLLISION:	// Touching a left wall collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_RIGHT_WALL:
		case TELEPORT:
			current_state = SLIDING_ON_LEFT_WALL;
			y_jumping_state = JST_UNKNOWN_Y;
			break;
		}
		break;
	case LEFT_UPPER_COLLISION:
		switch (current_state) {
		case FREE_JUMP:
		case FREE_FALLING:
		case WALL_JUMP:
		case SLIDING_ON_RIGHT_WALL:
		case TELEPORT:
			current_state = SLIDING_ON_LEFT_WALL;
			y_jumping_state = JST_UNKNOWN_Y;
			break;
		}
		break;
	case GROUND_COLLISION:	// Touching the ground collisions
		switch (current_state) {
		case FREE_FALLING:
		case WALL_JUMP:
		case TELEPORT:
			current_state = IDLE;
			y_jumping_state = JST_UNKNOWN_Y;
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
			y_jumping_state = JST_UNKNOWN_Y;
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
	if (current_state != DYING && (App->collision->DeathColliderTouched() == true || pos.y - hitbox_w_h.y > App->map->data.height * App->map->data.tile_height) || App->entity_m->kill == true) {
		current_state = DYING;
		App->audio->PlayFx(App->scene->death_sfx, 0);
	}
	if (current_state == DYING) {
		App->entity_m->kill = false;
		if (currentAnim->data->GetAnimationFinish()) { 
			App->level_m->RestartLevel();
		}
	}
	if (App->collision->DoorColliderTouched() == true) { App->level_m->ChangeToNextLevel(); }
}

void Player::MovePlayer(float dt) {

	// X AXIS MOVEMENT
	switch (current_state) {
	case WALK_FORWARD:
	case RUN_FORWARD:
		if (current_state == WALK_FORWARD) { pos.x += (speed.x / 2) * dt; }
		else { pos.x += speed.x * dt; }
		break;
	case WALK_BACKWARD:
	case RUN_BACKWARD:
		if (current_state == WALK_BACKWARD) { pos.x += -(speed.x / 2) * dt; }
		else { pos.x += -speed.x * dt; }
		break;
	case FREE_JUMP:
	case FREE_FALLING:
	case WALL_JUMP:
		JumpMoveX(dt);
		break;
	}

	// Y AXIS MOVEMENT
	if (current_state != DYING) {
		JumpMoveY(dt);
		switch (current_state) {
		case SLIDING_ON_LEFT_WALL:
		case SLIDING_ON_RIGHT_WALL:
			pos.y += (speed.y / 2.0f) * dt;
			break;
		}

		CheckMapBorder();
		AvoidShaking();
	}
}

void Player::JumpMoveX(float dt) {
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
			if (x_jumping_state == JST_GOING_LEFT && wall_jump_timer < 0.35f) {
				pos.x += -speed.x * dt;
			}
			else if (x_jumping_state == JST_GOING_RIGHT && wall_jump_timer < 0.35f) {
				pos.x += speed.x * dt;
			}
			else if (x_jumping_state == JST_IDLE) {}


			if (wall_jump_extra_move == SST_JUMPING_LEFT && wall_jump_timer > 0.0f) {
				pos.x += -wall_jump_timer*(speed.x*dt);
				wall_jump_timer -= dt;
			}
			else if (wall_jump_extra_move == SST_JUMPING_RIGHT && wall_jump_timer > 0.0f) {
				pos.x += wall_jump_timer*(speed.x*dt);
				wall_jump_timer -= dt;
			}
			break;
		default:
			if (x_jumping_state == JST_GOING_LEFT) { pos.x += -speed.x * dt; }
			else if (x_jumping_state == JST_GOING_RIGHT) { pos.x += speed.x * dt; }
			else if (x_jumping_state == JST_IDLE) {}
			break;
		}
		break;
	}
}

void Player::JumpMoveY(float dt) {

	switch (y_jumping_state) {
	case JST_UNKNOWN_Y:
		if (current_state == FREE_JUMP || current_state == WALL_JUMP) {
			y_jumping_state = JST_GOING_UP;
			jump_timer = 0.0f;
		}
		else if (current_state == FREE_FALLING) {
			y_jumping_state = JST_GOING_DOWN;
			jump_timer = 1.0f;
		}
		break;
	case JST_GOING_UP:
		if (jump_timer >= 0.0f && jump_timer < 1.0f) {
			jump_timer += dt;
			pos.y += (-speed.y * dt) + jump_timer*(speed.y * dt);
		}
		else { jump_timer = 1.0f; y_jumping_state = JST_GOING_DOWN; }
		break;
	case JST_TRANSITION:
		jump_timer = speed.y * dt;
		y_jumping_state = JST_GOING_DOWN;
		break;
	case JST_GOING_DOWN:
		if (current_state != FREE_FALLING && current_state != FREE_JUMP) {
			y_jumping_state = JST_UNKNOWN_Y;
			jump_timer = 0.0f;
		}
		else if (jump_timer <=2.0f) {
			if (jump_timer > 0.0f)
			{ jump_timer -= dt; }
			pos.y += (speed.y * dt) - jump_timer * (speed.y * dt);
		}
		break;
	}
}

void Player::AvoidShaking() {

	// This switch prevents shaking when colliding happens
	switch (App->collision->current_collision) {
	case GROUND_COLLISION:
		pos.y = App->collision->current_collision_buffer.collider1.y;
		if (current_state == FREE_JUMP) { pos.y += -1; }
		break;
	case UPPER_COLLISION:
		pos.y = App->collision->current_collision_buffer.collider1.y;
		if (current_state == FREE_FALLING) { pos.y += 1; }
		break;
	case LEFT_COLLISION:
	case RIGHT_COLLISION:
		pos.x = App->collision->current_collision_buffer.collider1.x + hitbox_w_h.x / 2;
		switch (wall_jump) {
		case SST_JUMPING_RIGHT:
			pos.x += 1.0f;
			pos.y += -1.0f;
			current_state = WALL_JUMP;
			break;
		case SST_JUMPING_LEFT:
			pos.x += -1.0f;
			pos.y += -1.0f;
			current_state = WALL_JUMP;
			break;
		case SST_FALLING_RIGHT:
			pos.x += 1.0f;
			pos.y += 1.0f;
			break;
		case SST_FALLING_LEFT:
			pos.x += -1.0f;
			pos.y += 1.0f;
			break;
		default:
			break;
		}
		break;
	case LEFT_GROUND_COLLISION:
	case RIGHT_GROUND_COLLISION:
		if (current_state != WALK_BACKWARD && current_state != WALK_FORWARD && current_state != FREE_JUMP) {
			if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
				pos.x = App->collision->current_collision_buffer.collider2.x + hitbox_w_h.x / 2;
				pos.y = App->collision->current_collision_buffer.collider1.y;
			}
			else if (App->collision->current_collision_buffer.is_first_collider_horizontal == false) {
				pos.x = App->collision->current_collision_buffer.collider1.x + hitbox_w_h.x / 2;
				pos.y = App->collision->current_collision_buffer.collider2.y;
			}
		}
		break;
	case LEFT_UPPER_COLLISION:
	case RIGHT_UPPER_COLLISION:
		if (current_state != SLIDING_ON_LEFT_WALL && current_state != SLIDING_ON_RIGHT_WALL) {
			if (App->collision->current_collision_buffer.is_first_collider_horizontal == true) {
				pos.x = App->collision->current_collision_buffer.collider2.x + hitbox_w_h.x / 2;
				pos.y = App->collision->current_collision_buffer.collider1.y;
			}
			else if (App->collision->current_collision_buffer.is_first_collider_horizontal == false) {
				pos.x = App->collision->current_collision_buffer.collider1.x + hitbox_w_h.x / 2;
				pos.y = App->collision->current_collision_buffer.collider2.y;
			}
		}
		switch (wall_jump) {
		case SST_FALLING_RIGHT:
			pos.x += 1.0f;
			pos.y += 1.0f;
			break;
		case SST_FALLING_LEFT:
			pos.x += -1.0f;
			pos.y += 1.0f;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (current_state != WALL_JUMP) {
		wall_jump_timer = 0.75f; //seconds
		wall_jump_extra_move = SST_IDLE;
	}
}

void Player::CheckMapBorder() {
	if (pos.x <= 5) {
		switch (App->collision->current_collision) {
		case GROUND_COLLISION:
		case LEFT_GROUND_COLLISION:
		case RIGHT_GROUND_COLLISION:
			current_state = IDLE;
			pos.x = 5;
			break;
		default:
			pos.x = 5;
			x_jumping_state = JST_IDLE;
			break;
		}
	}
}

bool Player::FlipCharacter(fPoint currentpos, fPoint lastpos)
{
	bool pfliped = fliped;
	if (currentpos.x < lastpos.x)pfliped = true;
	else if (currentpos.x > lastpos.x)pfliped = false;


	if (App->collision->LeftCollision())pfliped = true;
	if (App->collision->RightCollision())pfliped = false;

	return pfliped;
}

void Player::ChangeAnimation(Animation_list animations)
{
	p2List<Animations*>* pAnimList = &animations_list;//pointer to the player's animation list //TODO this is now loaded directly from the class, no need for a pointer
	p2List_item<Animations*>* currentanim = currentAnim;//pointer to the current animation//TODO this is now loaded directly from the class, no need for a pointer

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

	currentAnim = newanim;//sets the current animation of the player

}

void Player::CheckAnimation(state_list currentstate, state_list laststate)
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

bool Player::GetGodmode() {
	if (god_mode == true) { return true; }
	else { return false; }
}

void Player::SetGodmode() {
	god_mode = !god_mode;
}

void Player::SetGodmode(bool state) {
	if (state == true) { god_mode = true; }
	else if (state == false) { god_mode = false; }
}

void Player::GodMode(float dt) {
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		pos.x += -(speed.x * dt) * 4;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		pos.x += (speed.x * dt) * 4;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		pos.y += -(speed.y * dt) * 4;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		pos.y += (speed.y * dt) * 4;
	}
}
