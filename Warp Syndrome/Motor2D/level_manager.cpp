#include "level_manager.h"

j1LevelManager::j1LevelManager(){}

j1LevelManager::~j1LevelManager(){}

bool j1LevelManager::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1LevelManager::Start()
{
	return true;
}

bool j1LevelManager::PreUpdate()
{
	return true;
}

bool j1LevelManager::Update(float dt)
{
	return true;
}

bool j1LevelManager::PostUpdate()
{
	return true;
}

bool j1LevelManager::CleanUp()
{
	return true;
}

//Save/Load
bool j1LevelManager::Load(pugi::xml_node& ldata)
{
	return true;
}
bool j1LevelManager::Save(pugi::xml_node& ldata) const
{
	return true;
}