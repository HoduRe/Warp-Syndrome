#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Map.h"
#include "Enemy.h"
#include "PugiXml/src/pugixml.hpp"

struct collider_buffer {
	iPoint collider1;
	iPoint collider2;
	bool is_first_collider_horizontal = false;
};

enum object_colliding {
	OBJECT_UNKNOWN,
	OBJECT_PLAYER,
	OBJECT_GRENADE,
	OBJECT_ENEMY
};

enum collider_type {
	regular_collider,
	grenade_collider,
	door_collider,
	under_platform_collider,
	starting_point,
	death_collider,
	enemy_elemental,
	enemy_horse,
	enemy_skull,
	unknown_collider
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

	// sets a pointer to the object group
	void SetPointerToObjGroup(p2List<ObjectGroup*> &pointerObjList);

	// does the loop to check all colliders with the object recieved
	void CheckLoop(fPoint* position, fPoint* measures, object_colliding object_name);	// Exists because it's also used in the grenade

	// checks if the collider recieved is in contact with the object
	collision_type CheckCollision(p2List_item<Object*>* currentobj, float *x, float *y, float *w, float *h);

	//returns true if the two rectangles are overlapping
	bool CheckCollisionSimplified(SDL_Rect* r1, SDL_Rect* r2);

	// checks all collisions to return collisions in both axis, if they exist
	collision_type CalculateFinalCollision(collision_type collision_array[], collision_type current_collision);

	// return true if the object is inside a collider
	bool InsideCollider(p2List_item<Object*>* currentobj, float* x, float* y, float* w, float* h);

	// returns true if the object is in contact with a death_collider
	bool DeathColliderTouched();

	// returns true if the object is in contact with a door_collider
	bool DoorColliderTouched();

	// returns true if the grenade is inside a grenade_collider
	bool GrenadeColliderTouched();

	//returns true if a ground collision is happening
	bool GroundCollision();

	//returns true if a left collision is happening
	bool LeftCollision();

	//returns true if a right collision is happening
	bool RightCollision();

	// checks what collision goes first for the AvoidShaking function (moving the player out of the collision)
	void CollisionPriority(float collider_x, float collider_y, bool horizontal_collider);

	// If player has pressed F9, prints colliders
	void PrintColliders();
	
	// gets the type of the current collider
	collider_type GetCurrentCollider(int id);

	// Checks collision for pathfinding
	bool CheckWalkability(iPoint& tile);

public:
	collision_type current_collision;
	collider_buffer current_collision_buffer;

private:
	fPoint position;
	fPoint measures;
	int y_player_buffer;
	collider_type current_collider_type;
	p2List<ObjectGroup*> pObjGroupList;
	bool has_already_collided;
	bool death_collider_touched;
	bool door_collider_touched;
	bool grenade_collider_touched;
	collision_type collision_array[LAST_COLLISION];
};

#endif // __j1COLLISION_H__
