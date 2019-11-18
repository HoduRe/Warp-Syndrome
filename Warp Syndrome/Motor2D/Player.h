#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "j1Module.h"
#include "p2List.h"
#include "Animations.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

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
	TELEPORT,
	SLIDING_ON_RIGHT_WALL,
	SLIDING_ON_LEFT_WALL,
	WALL_JUMP,
	SLIDING_TO_IDLE,
	WAKE_UP,
	DYING,
	GOD_MODE
};

enum Animation_list
{
	AL_IDLE,
	AL_CROUCH,
	AL_RUNNING,
	AL_CROUCHING,
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
	AL_UNKNOWN
};

enum Animation_state
{
	AS_UNFINISHED,
	AS_FINISHED,
	AS_UNKNOWN
};

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

	bool LoadAnimations(pugi::xml_node&);

	bool ResetPlayerToStart();

	// Gives a state based on inputs
	void CheckInputs();
	// Checks Colliders
	void CheckCollisions();
	// Moves player position
	void MovePlayer();
	// Puts the proper animation
	void ChangeAnimation(Animation_list animations);
	//flips the player
	bool FlipPlayer(fPoint currentpos, fPoint lastpos);

	Animation_state StepCurrentAnimation();
	// Calculates jump shinanigans
	void JumpMoveX();
	void JumpMoveY();
	// Avoids shaking when going inside a collider
	void AvoidShaking();
	// Calculates if player is falling through map borders, and corrects it
	void CheckMapBorder();
	// Moves player if it is in God Mode
	void GodMode();
	// Checks if the animation has to be changed
	void CheckAnimation(state_list currentstate, state_list laststate);
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

public:
	FrameInfo* currentframe;
	p2List_item<Animations*>* currentAnim;
	state_list current_state;
	iPoint hitbox_w_h;
	p2List<Animations*> playerAnimations;

private:
	SDL_Event events;
	pugi::xml_document playerdoc;
	pugi::xml_node playernode;
	pugi::xml_node grenadenode;
	p2SString filename;

	bool grenade;
	bool god_mode;
	bool blit_colliders;
	int run_counter = 0;
	int death_counter = 0;
	float wall_jump_timer = 0;
	float jump_timer;
	SlidingStates wall_jump;
	SlidingStates wall_jump_extra_move;
	JumpingStatesX x_jumping_state;
	JumpingStatesY y_jumping_state;
	state_list bufferlaststate;
};


#endif // !__PLAYER_H__

