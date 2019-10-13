#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__
#include "j1Module.h"
#include "p2List.h"
#include "Animations.h"
#include "SDL/include/SDL.h"

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
	void SetPosition(float x, float y);

	void AddPosition(fPoint pos);
	void AddPosition(float x, float y);

	fPoint GetPosition();
	fPoint GetVelocity();

	bool GetFliped();

	void j1Player::SetFliped(bool flip);


	bool LoadAnimations(pugi::xml_node&);

	fPoint playerpos;	// TODO Hacer una función que altere la posición para la máquina de estados

private:
	fPoint playervel;
	p2List<Animations*> playerAnimations;
	SDL_Event events;
	pugi::xml_document playerdoc;
	pugi::xml_node playernode;
	p2SString filename;
	SDL_Texture* playertexture;
	bool rotated = false;
};


#endif // !__j1PLAYER_H__

