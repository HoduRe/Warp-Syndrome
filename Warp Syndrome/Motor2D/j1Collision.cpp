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
	collision_type collision_array[LAST_COLLISION] = { NONE_COLLISION, NONE_COLLISION, NONE_COLLISION, NONE_COLLISION,
	LEFT_UPPER_COLLISION, RIGHT_UPPER_COLLISION, NONE_COLLISION, LEFT_GROUND_COLLISION, RIGHT_GROUND_COLLISION, GROUND_UPPER_COLLISION, };
	// This array will check which collisions are happening, and change the value from NONE_COLLISION to the actual one
	//iterates all the object layers, and then iterates between all the object each layer contains
	p2List_item<ObjectGroup*>* itemOG = pObjGroupList.start;
	while (itemOG!=NULL)
	{
		p2List_item<Object*>* itemO = itemOG->data->objlist.start;
		while (itemO!=NULL)
		{
			current_collision = CheckCollider(itemO);
			collision_array[current_collision] = current_collision;

			itemO = itemO->next;
		}


		itemOG=itemOG->next;
	}
	current_collision = GetCollisionType(collision_array, current_collision);

	return ret;
}

bool j1Collision::CleanUp() {

	return true;
}

collision_type j1Collision::CheckCollider(p2List_item<Object*>* currentobj) {

	//player variables
	float x = App->player->GetPosition().x;
	float y = App->player->GetPosition().y;
	float w = 3;	// TODO use character height to differenciate x axis from x_y axis
	float h = 3;	// TODO use character height to differenciate x axis from x_y axis

	//collider variables
	float collider_x = (float)currentobj->data->boundingbox.x;
	float collider_y = (float)currentobj->data->boundingbox.y;
	float collider_w = (float)currentobj->data->boundingbox.w;
	float collider_h = (float)currentobj->data->boundingbox.h;

	if (collider_y <= y + h && collider_y > y && collider_x < x && collider_x + collider_w > x) {
		return GROUND_COLLISION;
	}
	else if (collider_x >= x + w && collider_y < y + h && collider_y + collider_h > y + h) {
		return RIGHT_COLLISION;
	}
	else if (collider_x + collider_w <= x && collider_y < y + h && collider_y + collider_h > y + h) {
		return LEFT_COLLISION;
	}
	else if (collider_y + collider_h >= y && collider_y + collider_h <= y + h && collider_x < x && collider_x + collider_w > x) {
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
			collision_count = collision_array[i];
			break;
		}
	}

	return current_collision = collision_array[collision_count];
}


//Called from the Map module every time a map is loaded
void j1Collision::SetPointerToObjGroup(p2List<ObjectGroup*> &pointerObjGroupList)
{
	pObjGroupList = pointerObjGroupList;
}