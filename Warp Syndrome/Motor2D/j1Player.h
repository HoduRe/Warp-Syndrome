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

	void SetFliped(bool flip);

	p2List<Animations*>* GetAnimationList();
	
	p2List_item<Animations*>* GetCurrentAnim();

	FrameInfo* GetCurrentFrame();
	void SetCurrentFrame(FrameInfo* frame);

	void SetCurrentAnim(p2List_item<Animations*>* animationpointer);

	SDL_Texture* GetTexture();

	iPoint GetWidthHeight();

	bool LoadAnimations(pugi::xml_node&);

	bool ResetPlayerToStart();


private:
	fPoint playerpos;
	fPoint playervel;
	iPoint hitbox_w_h;
	p2List<Animations*> playerAnimations;
	p2List_item<Animations*>* currentAnim;
	FrameInfo* currentframe;
	SDL_Event events;
	pugi::xml_document playerdoc;
	pugi::xml_node playernode;
	p2SString filename;
	SDL_Texture* playertexture;
	bool fliped;
};


#endif // !__j1PLAYER_H__

