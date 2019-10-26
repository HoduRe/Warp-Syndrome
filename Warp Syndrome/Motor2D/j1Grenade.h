#ifndef __j1GRENADE_H__
#define __j1GRENADE_H__

#include "j1Module.h"
#include "j1Collision.h"
#include "p2List.h"

enum grenade_states {
	GST_UNKNOWN,
	GST_MOVING_UP,
	GST_MOVING_DOWN,
	GST_MOVING_RIGHT_UP,
	GST_MOVING_RIGHT_DOWN,
	GST_MOVING_LEFT_UP,
	GST_MOVING_LEFT_DOWN,
	GST_EXPLODING
};

class j1Grenade : public j1Module
{
public:

	j1Grenade();

	// Destructor
	~j1Grenade();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Chechk collisions
	void GrenadeCollisions();

	// Simple machine state for the grenade
	void GrenadeState();

	// Checks that the grenade is not going out of the map
	void CheckMapBorder();

	// return true if the grenade exists
	bool DoesGrenadeExist();

	// returns true if the grenade is in GST_EXPLODING
	bool IsGrenadeExploding();

	// adds the numbers given to the current grenade position
	void AddPosition(float x, float y);

	// returns the position
	fPoint GetPosition();

	// returns the width and height of the grenade
	fPoint GetMeasures();

	collision_type ColliderBuffer();

	//returns true if granade is in cooldown, false if its not
	bool GetGrenadeCooldown();

	void StepGrenadeCooldown();

	void GrenadeCooldownReset();

	void SetMeasures(pugi::xml_node root);

private:
	fPoint grenade_position, grenade_timer, grenade_vel, grenade_measures;
	grenade_states grenade_state = GST_UNKNOWN;
	collision_type grenade_collider_buffer;
	SDL_Texture* grenade_texture;
	p2List<Animations*>* anim_list;
	p2List_item<Animations*>* grenade_animation;
	float grenade_time_to_explode;
	int cooldown_timer = 0;

};

#endif // __j1GRENADE_H__