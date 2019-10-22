#include "transitions.h"

j1Transitions::j1Transitions() {}

j1Transitions::~j1Transitions() {}

bool j1Transitions::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Transitions::Start()
{
	return true;
}

bool j1Transitions::PreUpdate()
{
	return true;
}

bool j1Transitions::Update(float dt)
{
	return true;
}

bool j1Transitions::PostUpdate()
{
	return true;
}

bool j1Transitions::CleanUp()
{
	return true;
}

//Save/Load
bool j1Transitions::Load(pugi::xml_node& ldata)
{
	return true;
}
bool j1Transitions::Save(pugi::xml_node& ldata) const
{
	return true;
}