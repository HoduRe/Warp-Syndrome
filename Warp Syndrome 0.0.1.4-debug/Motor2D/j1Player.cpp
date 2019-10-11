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
{
	name.create("player");
}
j1Player::~j1Player()
{}



bool j1Player::Awake(pugi::xml_node& config)
{
	filename.create(config.child("load").attribute("docname").as_string());
	return true;
};


bool j1Player::Start()
{
	bool ret = true;
	pugi::xml_parse_result result = playerdoc.load_file(filename.GetString());

	if (result == NULL)
	{
		LOG("Could not load player documant. pugi error: %s", result.description());
		ret = false;
	}

	playernode = playerdoc.child("player");
	for (pugi::xml_node animationnode = playernode.child("animation"); animationnode; animationnode = animationnode.next_sibling("animation"))
	{
		Animations *anim=new Animations;
		anim->LoadAnim(animationnode);
		//p2List_item<Animations*>* anim_item= anim;
		
		playerAnimations.add(anim);
	}
	return ret;
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
	//TODO Un-comment this and see if it works once playerAnimations is inicialized
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

void j1Player::SetPosition(fPoint pos)
{
	playerpos = pos;
}
void j1Player::Setposition(float x, float y)
{
	playerpos.x = x;
	playerpos.y = y;
}