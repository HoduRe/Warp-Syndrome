#include "j1App.h"
#include "j1UIManager.h"
#include "p2Log.h"

j1UIManager::j1UIManager() : j1Module() { name.create("ui_m"); }

// Destructor
j1UIManager::~j1UIManager() {}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& config)
{
	LOG("Loading UIManager");
	bool ret = true;

	return ret;
}

bool j1UIManager::Update(float dt) {

	return true;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Unloading UIManager");

	return true;
}
