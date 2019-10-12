#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2List.h"
#include "j1App.h"

j1Collision::j1Collision() : j1Module() {
	name.create("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Awake(pugi::xml_node& config) {
	//filename.create(config.child("load").attribute("docname").as_string());
	return true;
}

bool j1Collision::Start()
{

	return true;
}

bool j1Collision::Update(float dt) {
	
	bool ret = true;
	if (changemap == true) { ret = ChargeMapName(); }
	
	collision_type collision_array[LAST_COLLISION] = {NONE_COLLISION, NONE_COLLISION, NONE_COLLISION, NONE_COLLISION,
	LEFT_UPPER_COLLISION, RIGHT_UPPER_COLLISION, NONE_COLLISION, LEFT_GROUND_COLLISION, RIGHT_GROUND_COLLISION, GROUND_UPPER_COLLISION, };
	// This array will check which collisions are happening, and change the value from NONE_COLLISION to the actual one
	
	for (collider_node = map_file.child("objectgroup").child("obect"); collider_node && ret; collider_node = collider_node.next_sibling()){
		current_collision = CheckCollider(collider_node);
		collision_array[current_collision] = current_collision;
	}
	
	current_collision = GetCollisionType(collision_array, current_collision);

	return ret;
}

bool j1Collision::CleanUp() {

	return true;
}

collision_type j1Collision::CheckCollider(pugi::xml_node& collider_node) {

	//player variables
	float x = App->player->playerpos.x;
	float y = App->player->playerpos.y;
	float player_w = 0;	// TODO use character height to differenciate x axis from x_y axis
	float player_h = 0;	// TODO use character height to differenciate x axis from x_y axis

	//collider variables
	float collider_x = collider_node.attribute("x").as_float() / App->map->data.tile_width;
	float collider_y = collider_node.attribute("y").as_float() / App->map->data.tile_height;
	float collider_w = collider_node.attribute("width").as_float() / App->map->data.tile_width;
	float collider_h = collider_node.attribute("height").as_float() / App->map->data.tile_height;
	
	if (collider_y == y + player_h && collider_x < x &&	collider_x + collider_w > x) {
		return GROUND_COLLISION;
	}
	else if (collider_x == x + player_w && collider_y < y + player_h && collider_y + collider_h > y + player_h) {
		return RIGHT_COLLISION;
	}
	else if (collider_x + collider_w == x && collider_y < y + player_h && collider_y + collider_h > y + player_h) {
		return LEFT_COLLISION;
	}
	else if (collider_y + collider_h == y && collider_x < x && collider_x + collider_w > x) {
		return UPPER_COLLISION;
	}
	else { return NONE_COLLISION; }
}

collision_type j1Collision::GetCollisionType(collision_type collision_array[], collision_type current_collision) {
	int collision_count = 0;

	for (int i = 0; i < LAST_COLLISION; i++) {
		if (collision_array[i] != NONE_COLLISION && collision_array[i] != LEFT_UPPER_COLLISION &&
			collision_array[i] != RIGHT_UPPER_COLLISION && collision_array[i] != LEFT_GROUND_COLLISION &&
			collision_array[i] != RIGHT_GROUND_COLLISION && GROUND_UPPER_COLLISION) {
			collision_count = collision_array[i];
		}
	}

	return current_collision = collision_array[collision_count];
}

bool j1Collision::ChargeMapName() {
	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());
	if (result == NULL) {
		LOG("Could not load map xml file. pugi error: %s", result.description());
		return false;
	}
	changemap = false;
	return true;
}