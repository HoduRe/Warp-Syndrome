#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "j1Module.h"
#include "p2List.h"
#include "Animations.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

//this class holds all the data for the player
class Player : public Entity
{

public:
	Player();
	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);//TODO it not longer works with node

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
	void SetPosition(float x, float y);

	void AddPosition(fPoint pos);
	void AddPosition(float x, float y);

	fPoint GetPosition();

	p2List<Animations*>* GetAnimationList();

	SDL_Texture* GetTexture();

	iPoint GetWidthHeight();

	bool LoadAnimations(pugi::xml_node&);

	bool ResetPlayerToStart();

public:
	FrameInfo* currentframe;
	p2List_item<Animations*>* currentAnim;

private:
	iPoint hitbox_w_h;
	p2List<Animations*> playerAnimations;
	SDL_Event events;
	pugi::xml_document playerdoc;
	pugi::xml_node playernode;
	pugi::xml_node grenadenode;
	p2SString filename;
};


#endif // !__PLAYER_H__

