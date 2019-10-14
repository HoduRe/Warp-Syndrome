#ifndef __j1STATE_H__
#define __j1STATE_H__

#include "j1Module.h"
#include "p2List.h"

enum state_list {
	NONE,
	IDLE,
	WALK_FORWARD,
	WALK_BACKWARD,
	RUN_FORWARD,
	RUN_BACKWARD,
	FREE_JUMP,
	FREE_FALLING,
	THROWING_GRENADE,
	THROWING_GRENADE_ON_AIR,
	TELEPORT,
	SLIDING_ON_RIGHT_WALL,
	SLIDING_ON_LEFT_WALL,
	WALL_JUMP,
	SLIDING_TO_IDLE,
	WAKE_UP,
	DEAD,
	GOD_MODE
};

class j1State : public j1Module
{
public:

	j1State();

	// Destructor
	virtual ~j1State();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// transitions automatic states, like jumping->falling->idle
	void ChangeState();
	// Gives a state based on inputs
	void CheckInputs();
	// Checks Colliders
	void CheckColliders();
	// Moves player position
	void MovePlayer();
	// Puts the proper animation
	void ChangeAnimation();

	void JumpMove();

	state_list current_state;
	bool animation_end = false;

private:
	bool double_jump = true;
	bool grenade = false;
	int run_counter = 0;
	int internal_counter = 0;
	float jump_timer;
};

#endif // __j1STATE_H__
