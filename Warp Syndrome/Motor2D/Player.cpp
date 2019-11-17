#include "p2Defs.h"
#include "p2Log.h"
#include "p2SString.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1State.h"
#include "j1Map.h"
#include "Player.h"
#include "Entity.h"


#include "SDL/include/SDL.h"


Player::Player():Entity(EntityType::E_TYPE_PLAYER)
{
	currentframe=nullptr;
	currentAnim=nullptr;
	hitbox_w_h = {0,0};
	playertexture=nullptr;
	//creates the name of the class for usage to target the right node of coniguration in the awake function
	//name.create("player");
}

Player::~Player()
{

}



bool Player::Awake(pugi::xml_node& config)
{
	//gets the file name of the player document from the config document
	filename.create(config.child("load").attribute("docname").as_string());
	return true;
};


bool Player::Start()
{

	bool ret = true;
	fliped = false;
	//loads the player document
	pugi::xml_parse_result result = playerdoc.load_file(filename.GetString());

	if (result == NULL)
	{
		LOG("Could not load player documant. pugi error: %s", result.description());
		ret = false;
	}

	//initializes playernode to be the root node of the doc

	playernode = playerdoc.child("player");
	/*grenadenode = playernode.child("grenade");
	App->grenade->SetMeasures(grenadenode);*/
	LoadAnimations(playernode);

	pugi::xml_node texturenode = playernode.child("texture");

	ResetPlayerToStart();
	speed.x = playernode.child("velocity").attribute("x").as_float();
	speed.y = playernode.child("velocity").attribute("y").as_float();
	hitbox_w_h.x = playernode.child("hitbox").attribute("w").as_int();
	hitbox_w_h.y = playernode.child("hitbox").attribute("h").as_int();
	
	//Load image
	playertexture = App->tex->Load(PATH(texturenode.child("folder").text().as_string(), texturenode.child("load").attribute("texturename").as_string()));

	p2List_item<Animations*>* defaultanim = playerAnimations.start->data->GetAnimFromName("idle", &playerAnimations);
	currentAnim = defaultanim;
	return ret;
}

bool Player::PreUpdate()
{
	return true;
}
bool Player::Update(float dt)
{
	return true;
}

bool Player::PostUpdate()
{
	if (currentframe != NULL)
		App->render->Blit(playertexture, pos.x, pos.y - currentframe->animationRect.h - currentframe->textureoffset.y, &currentframe->animationRect, fliped, currentframe->textureoffset.x);

	return true;
}

// Called before quitting
bool Player::CleanUp()
{
	App->tex->UnLoad(playertexture);
	p2List_item<Animations*>* item;
	item = playerAnimations.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	playerAnimations.clear();
	return true;
}

// Load / Save
bool Player::Load(pugi::xml_node& data)
{
	pos.x = data.attribute("x").as_int(0);
	pos.y = data.attribute("y").as_int(0);

	return true;
}
bool Player::Save(pugi::xml_node& data) const
{

	data.append_attribute("x") = pos.x;
	data.append_attribute("y") = pos.y;
	return true;
}

void Player::SetPosition(fPoint pos)
{
	pos = pos;
}

void Player::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

//Loads all the animations needed for the player
//returns false if no animation has been loaded, otherwise returns true
bool Player::LoadAnimations(pugi::xml_node& rootnode)
{
	bool ret = false;
	for (pugi::xml_node animationnode = rootnode.child("animation"); animationnode; animationnode = animationnode.next_sibling("animation"))
	{
		Animations* anim = new Animations;
		ret = anim->LoadAnim(animationnode);

		playerAnimations.add(anim);
	}
	return ret;
}

void Player::AddPosition(fPoint pos)
{
	pos += pos;
}

void Player::AddPosition(float x, float y)
{
	pos.x += x;
	pos.y += y;
}

fPoint Player::GetPosition()
{
	return  pos;
}



p2List<Animations*>* Player::GetAnimationList()
{
	return &playerAnimations;
}


SDL_Texture* Player::GetTexture() {
	return playertexture;
}

iPoint Player::GetWidthHeight() {
	return hitbox_w_h;
}

bool Player::ResetPlayerToStart()
{
	p2List_item<Object*>* startingpoint = nullptr;
	//search for the player spawn point
	bool found = false;
	//for every group
	p2List_item<ObjectGroup*>* group = App->map->data.objgroups.start;
	while (group != NULL && !found)
	{
		//for every item in this group
		p2List_item<Object*>* object = group->data->objlist.start;
		while (object != NULL && !found)
		{
			if (object->data->type == 4)
			{
				startingpoint = object;
				found = true;

			}
			object = object->next;
		}
		group = group->next;
	}

	if (startingpoint != NULL)
	{
		pos.x = startingpoint->data->boundingbox.x;
		pos.y = startingpoint->data->boundingbox.y;
	}
	else //default in case nothing loads
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
	}

	App->state->current_state = IDLE;
	App->state->SetGrenadeState(false);

	return true;
}
