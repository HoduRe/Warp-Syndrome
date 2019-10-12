#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include "p2List.h"
#include "PugiXml/src/pugixml.hpp"

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

	collision_type CheckCollider(pugi::xml_node& collider_node);
	collision_type GetCollisionType(collision_type collision_array[], collision_type current_collision);

	p2SString current_map_name;
	collision_type current_collision = NONE_COLLISION;

private:
	pugi::xml_document map_file;
	pugi::xml_node collider_node;
	p2SString filename;
};

#endif // __j1AUDIO_H__
