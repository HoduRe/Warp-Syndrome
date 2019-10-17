#include "p2Defs.h"
#include "p2Log.h"
#include "j1State.h"
#include "j1Collision.h"
#include "j1Grenade.h"

j1Grenade::j1Grenade() {
	name.create("grenade");
}

// Destructor
j1Grenade::~j1Grenade() {}

// Called before render is available
bool j1Grenade::Awake(pugi::xml_node&) {

	return true;
}

// Called before the first frame
bool j1Grenade::Start() {

	return true;
}

// Called each loop iteration
bool j1Grenade::Update(float dt) {

	return true;
}

// Called before quitting
bool j1Grenade::CleanUp() {

	return true;
}
