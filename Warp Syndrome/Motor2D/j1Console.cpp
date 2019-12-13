#include "j1App.h"
#include "j1Console.h"
#include "p2Log.h"

j1Console::j1Console() : j1Module() { name.create("console"); }

// Destructor
j1Console::~j1Console() {}

// Called before render is available
bool j1Console::Awake(pugi::xml_node& config)
{
	LOG("Loading console");
	bool ret = true;

	return ret;
}

bool j1Console::Update(float dt) {

	return true;
}

// Called before quitting
bool j1Console::CleanUp()
{
	LOG("Unloading console");

	return true;
}
