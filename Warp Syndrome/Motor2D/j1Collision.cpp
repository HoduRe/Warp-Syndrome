#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2List.h"
#include "j1App.h"

j1Collision::j1Collision() : j1Module() 
{
	name.create("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Awake(pugi::xml_node& config) {
	//filename.create(config.child("load").attribute("docname").as_string());
	return true;
}

bool j1Collision::Start()
{
	current_collision = NONE_COLLISION;
	return true;
}

bool j1Collision::Update(float dt) {

	bool ret = true;

	//player variables
	fPoint position;
	fPoint measures;
	position.x = App->player->GetPosition().x;
	position.y = App->player->GetPosition().y;
	measures.x = 20;	// TODO use character height to differenciate x axis from x_y axis
	measures.y = 30;	// TODO use character height to differenciate x axis from x_y axis
	CheckLoop(&position, &measures);

	return ret;
}

bool j1Collision::CleanUp() {

	return true;
}

void j1Collision::CheckLoop(fPoint *position, fPoint *measures) {
	
	p2List_item<ObjectGroup*>* itemOG = pObjGroupList.start;
	//Reset bools
	has_already_collided = false;
	current_collision_buffer.is_first_collider_horizontal = false;

	// This array will check which collisions are happening, and change the value from NONE_COLLISION to the actual one
	//iterates all the object layers, and then iterates between all the object each layer contains
	collision_array[0] = NONE_COLLISION;
	collision_array[1] = NONE_COLLISION;
	collision_array[2] = NONE_COLLISION;
	collision_array[3] = NONE_COLLISION;
	collision_array[4] = LEFT_UPPER_COLLISION;
	collision_array[5] = RIGHT_UPPER_COLLISION;
	collision_array[6] = NONE_COLLISION;
	collision_array[7] = LEFT_GROUND_COLLISION;
	collision_array[8] = RIGHT_GROUND_COLLISION;
	collision_array[9] = GROUND_UPPER_COLLISION;

	while (itemOG != NULL)
	{
		p2List_item<Object*>* itemO = itemOG->data->objlist.start;
		while (itemO != NULL)
		{
			current_collision = CheckCollider(itemO, &position->x, &position->y, &measures->x, &measures->y);
			collision_array[current_collision] = current_collision;

			itemO = itemO->next;
		}


		itemOG = itemOG->next;
	}
	current_collision = GetCollisionType(collision_array, current_collision);
}

collision_type j1Collision::CheckCollider(p2List_item<Object*>* currentobj, float *x, float *y, float *w,  float *h) {

	//collider variables
	float collider_x = (float)currentobj->data->boundingbox.x;
	float collider_y = (float)currentobj->data->boundingbox.y;
	float collider_w = (float)currentobj->data->boundingbox.w;
	float collider_h = (float)currentobj->data->boundingbox.h;

	if (collider_y <= *y && collider_y > *y - *h && collider_x < *x && collider_x + collider_w > *x) {
		GetBufferCollision(collider_x, collider_y, true);
		return GROUND_COLLISION;
	}
	else if (collider_x > *x && collider_x <= *x + *w && collider_y < *y && collider_y + collider_h > *y - *h) {
		GetBufferCollision(collider_x - *w, collider_y, false);
		return RIGHT_COLLISION;
	}
	else if (collider_x + collider_w < *x + *w && collider_x + collider_w >= *x && collider_y < *y && collider_y + collider_h > *y - *h) {
		GetBufferCollision(collider_x + collider_w, collider_y, false);
		return LEFT_COLLISION;
	}
	else if (collider_y + collider_h >= *y - *h && collider_y + collider_h <= *y && collider_x < *x && collider_x + collider_w > *x) {
		GetBufferCollision(collider_x, collider_y + collider_h + *h, true);
		return UPPER_COLLISION;
	}
	else { return NONE_COLLISION; }
}

collision_type j1Collision::GetCollisionType(collision_type collision_array[], collision_type current_collision) {
	int collision_count = 0;

	for (int i = 0; i < LAST_COLLISION; i++) {
		switch (collision_array[i]) {
		case NONE_COLLISION:
		case LEFT_COLLISION:
		case RIGHT_COLLISION:
		case UPPER_COLLISION:
		case GROUND_COLLISION:
			collision_count += collision_array[i];
			break;
		}
	}

	return current_collision = collision_array[collision_count];
}

// Gets the buffer collider to avoid the player going into a solid mass
void j1Collision::GetBufferCollision(float collider_x, float collider_y, bool horizontal_collider) {

	switch (has_already_collided) {
	case false:
		current_collision_buffer.collider1.x = (int)collider_x;
		current_collision_buffer.collider1.y = (int)collider_y;
		has_already_collided = true;
		if (horizontal_collider == true) { current_collision_buffer.is_first_collider_horizontal = true; }
		break;
	case true:
		current_collision_buffer.collider2.x = (int)collider_x;
		current_collision_buffer.collider2.y = (int)collider_y;
		break;
	}
}

//Called from the Map module every time a map is loaded
void j1Collision::SetPointerToObjGroup(p2List<ObjectGroup*> &pointerObjGroupList)
{
	pObjGroupList = pointerObjGroupList;
}
