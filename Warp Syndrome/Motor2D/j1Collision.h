#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Map.h"
#include "PugiXml/src/pugixml.hpp"

struct collider_buffer {
	iPoint collider1;
	iPoint collider2;
	bool is_first_collider_horizontal = false;
};

enum object_colliding {
	OBJECT_UNKNOWN,
	OBJECT_PLAYER,
	OBJECT_GRENADE
};

enum collider_type {
	regular_collider,
	grenade_collider,
	door_collider,
	starting_point,
	death_collider
};

enum collision_type {
	NONE_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION,
	UPPER_COLLISION,
	LEFT_UPPER_COLLISION,
	RIGHT_UPPER_COLLISION,
	GROUND_COLLISION,
	LEFT_GROUND_COLLISION,
	RIGHT_GROUND_COLLISION,	// The order of the enum matters, because we may recieve various collisions, and we'll sum
	GROUND_UPPER_COLLISION,	// their value to get a collision ocurring in two axis (if it's happening, that is)
	LAST_COLLISION,	// This has to be always the last collision so that other collision types can be added
};

class j1Collision : public j1Module
{
public:

	j1Collision();

	// Destructor
	virtual ~j1Collision();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void SetPointerToObjGroup(p2List<ObjectGroup*> &pointerObjList);

	void CheckLoop(fPoint* position, fPoint* measures, object_colliding object_name);	// Exists because it's also used in the grenade

	collision_type CheckCollider(p2List_item<Object*>* currentobj, float *x, float *y, float *w, float *h);
	collision_type GetCollisionType(collision_type collision_array[], collision_type current_collision);
	bool InsideCollider(p2List_item<Object*>* currentobj, float* x, float* y, float* w, float* h);
	bool DeathColliderTouched();
	bool DoorColliderTouched();
	bool GrenadeColliderTouched();
	//returns true if a ground collision is happening
	bool GroundCollision();
	void GetBufferCollision(float collider_x, float collider_y, bool horizontal_collider);
	void GetCurrentCollider(p2SString name);

	collision_type current_collision;
	collider_buffer current_collision_buffer;

private:
	fPoint position;
	fPoint measures;
	collider_type current_collider_type;
	p2List<ObjectGroup*> pObjGroupList;
	bool has_already_collided;
	bool death_collider_touched;
	bool door_collider_touched;
	bool grenade_collider_touched;
	collision_type collision_array[LAST_COLLISION];
};

#endif // __j1COLLISION_H__
