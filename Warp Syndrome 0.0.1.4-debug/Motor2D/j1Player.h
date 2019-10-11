#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__
#include "j1Module.h"
#include "p2List.h"
#include "Animations.h"



//enums all the stats in which the player can be at any given moment
//it is used in the Finite Stat Machine
enum PlayerStates
{
	ST_UNKNOWN,
	ST_IDLE,
	ST_WALKING,
	ST_RUNNING,
	ST_GOINGTOCROUCH,
	ST_CROUCHING,
	ST_JUMPING,
	ST_SLIDINGONWALL,
	ST_DEAD,
	ST_GODMODE,
	ST_TROWINGGRENADE
};

//enums all the imputs needed for the Finite Stat Machine
enum PlayerInputs
{
	IN_UNKNOWN,
	IN_BUTTON_UP,
	IN_BUTTON_DOWN,
	IN_BUTTON_LEFT,
	IN_BUTTON_RIGTH,
	IN_BUTTON_THROW,
	IN_FLOOR_COLLISION,
	IN_WALL_COLLISION,
	IN_GOD_MODE,
	IN_DEAD
};

//this class holds all the data for the player
class j1Player :public j1Module
{

public:
	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void SetPosition(fPoint pos);
	void Setposition(float x, float y);

private:
	fPoint playerpos;
	fPoint playervel;
	PlayerStates playerState;
	p2List<Animations*> playerAnimations;
	SDL_Event events;
	pugi::xml_document playerdoc;
	pugi::xml_node playernode;
	p2SString filename;
};











#endif // !__j1PLAYER_H__

