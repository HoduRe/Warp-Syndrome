#ifndef __j1STATE_H__
#define __j1STATE_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

enum SlidingStates {
	SST_JUMPING_LEFT,
	SST_JUMPING_RIGHT,
	SST_FALLING_LEFT,
	SST_FALLING_RIGHT,
	SST_IDLE
};
enum JumpingStatesX {
	JST_GOING_LEFT,
	JST_GOING_RIGHT,
	JST_IDLE
};

enum JumpingStatesY
{
	JST_GOING_UP,
	JST_TRANSITION,
	JST_GOING_DOWN,
	JST_UNKNOWN
};

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
	DYING,
	DEAD,
	GOD_MODE
};

enum Animation_list
{
	AL_IDLE,
	AL_CROUCH,
	AL_RUNNING,
	AL_CROUCHING,
	AL_THROW_GRANADE,
	AL_FORWARD_JUMP,
	AL_FALLING,
	AL_SLIDING,
	AL_HURT,
	AL_WALLSLIDING,
	AL_WALLSLIDING_TO_IDLE,
	AL_WALLJUMPING,
	AL_WAKEUP,
	AL_WALKING,
	AL_DYING,
	AL_DEAD,
	AL_UNKNOWN
};

enum Animation_state
{
	AS_UNFINISHED,
	AS_FINISHED,
	AS_UNKNOWN
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

	// Gives a state based on inputs
	void CheckInputs();
	// Checks Colliders
	void CheckColliders();
	// Moves player position
	void MovePlayer();
	// Puts the proper animation
	void ChangeAnimation(Animation_list animations);
	//flips the player
	bool FlipPlayer(fPoint currentpos,fPoint lastpos);

	Animation_state StepCurrentAnimation();


	// Calculates jump shinanigans
	void JumpMoveX();
	void JumpMoveY();

	// Avoids shaking when going inside a collider
	void AvoidShaking();

	// Moves player if it is in God Mode
	void GodMode();

	// Checks if the animation has to be changed
	void CheckAnimation(state_list currentstate, state_list laststate);

	state_list current_state;

	// returns true if the grenade exists
	bool GetGrenadeState();

	// changes the stata of the grenade
	void SetGrenadeState(bool state);

	// returns true if the player presses F9
	bool BlitColliders();

	// activates or deactivates the blit colliders F9
	void SetBlitColliders();

	// returns true if the player is in godmode
	bool GetGodmode();

	// sets god mode
	void SetGodmode();

	// changes godmode based on what is being requested
	void SetGodmode(bool state);
	
private:
	bool grenade;
	bool god_mode;
	bool blit_colliders;
	int run_counter;;
	float jump_timer;
	SlidingStates wall_jump;
	JumpingStatesX x_jumping_state;
	JumpingStatesY y_jumping_state;
};

#endif // __j1STATE_H__
