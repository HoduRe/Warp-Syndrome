#include "p2Defs.h"
#include "p2Log.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "j1Player.h"


j1Player::j1Player()
{}
j1Player::~j1Player()
{}



bool j1Player::Awake(pugi::xml_node& config)
{
	return true;
};


bool j1Player::Start()
{
	return true;
}
bool j1Player::PreUpdate()
{
	return true;
}
bool j1Player::Update(float dt)
{
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	//playerAnimations->clear();
	return true;
}

// Load / Save
bool j1Player::Load(pugi::xml_node& data) 
{
	return true;
}
bool j1Player::Save(pugi::xml_node& data) const
{
	return true;
}