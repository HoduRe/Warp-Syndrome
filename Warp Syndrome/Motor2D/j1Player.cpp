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
#include "SDL/include/SDL.h"
#include "j1Player.h"


j1Player::j1Player()
{
	//creates the name of the class for usage to target the right node of coniguration in the awake function
	name.create("player");
}

j1Player::~j1Player()
{
}



bool j1Player::Awake(pugi::xml_node& config)
{
	//gets the file name of the player document from the config document
	filename.create(config.child("load").attribute("docname").as_string());
	return true;
};


bool j1Player::Start()
{
	bool ret = true;
	//loads the player document
	pugi::xml_parse_result result = playerdoc.load_file(filename.GetString());

	if (result == NULL)
	{
		LOG("Could not load player documant. pugi error: %s", result.description());
		ret = false;
	}

	//initializes playernode to be the root node of the doc
	playernode = playerdoc.child("player");

	LoadAnimations(playernode);

	pugi::xml_node texturenode = playernode.child("texture");


	//Load image

	playertexture = App->tex->Load(PATH(texturenode.child("folder").text().as_string(), texturenode.child("load").attribute("texturename").as_string()));

	//TODO load those values from the xml instead of harcoding them
	playerpos.x = 100.0f;
	playerpos.y = 52.0f;
	playervel.x = 4.0f;
	playervel.y = 5.0f;
	
	p2List_item<Animations*>* defaultanim = playerAnimations.start->data->GetAnimFromName("idle", &playerAnimations);
	SetCurrentAnim(defaultanim);
	return ret;
}

bool j1Player::PreUpdate()
{
	return true;
}
bool j1Player::Update(float dt)
{
	// TODO hide the player when going behind the tower (or print the tower in the front, like in a parallax effect
	return true;
}

bool j1Player::PostUpdate()
{
	if (currentframe!=NULL)	
	App->render->Blit(playertexture, playerpos.x-currentframe->textureoffset.x, playerpos.y-currentframe->animationRect.h -currentframe->textureoffset.y, &currentframe->animationRect);

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(playertexture);
	playerAnimations.clear();
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

void j1Player::SetPosition(float x, float y)
{
	playerpos.x = x;
	playerpos.y = y;
}

//Loads all the animations needed for the player
//returns false if no animation has been loaded, otherwise returns true
bool j1Player::LoadAnimations(pugi::xml_node& rootnode)
{
	bool ret = false;
	for (pugi::xml_node animationnode = rootnode.child("animation"); animationnode; animationnode = animationnode.next_sibling("animation"))
	{
		Animations* anim = new Animations;
		ret= anim->LoadAnim(animationnode);

		playerAnimations.add(anim);
	}
	return ret;
}

void j1Player::AddPosition(fPoint pos)
{
	playerpos += pos;
}

void j1Player::AddPosition(float x, float y)
{
	playerpos.x += x;
	playerpos.y += y;
}

fPoint j1Player::GetPosition()
{
	return  playerpos;
}

//returns true if player is flipped
bool j1Player::GetFliped()
{
	return rotated;
}

void j1Player::SetFliped(bool flip)
{
	rotated = flip;
}

fPoint j1Player::GetVelocity()
{
	return playervel;
}

p2List<Animations*>* j1Player::GetAnimationList()
{
	return &playerAnimations;
}

void j1Player::SetCurrentAnim(p2List_item<Animations*>* animationpointer)
{
	currentAnim = animationpointer;
}

p2List_item<Animations*>* j1Player::GetCurrentAnim()
{
	return currentAnim;
}

FrameInfo* j1Player::GetCurrentFrame()
{
	return currentframe;
}
void j1Player::SetCurrentFrame(FrameInfo* frame)
{
	currentframe = frame;
}

