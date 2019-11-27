#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animations.h"
#include "p2DynArray.h"
#include "j1Collision.h"
#include "Entity.h"
#include "Character.h"
#include "p2DynArray.h"

struct SDL_Texture;

enum enemy_states
{
	E_STATE_DEFAULT,//normal enemy behaviour when doesn't see the player
	E_STATE_CHASING,
	E_STATE_ATTACKING,
	E_STATE_DIE,
	E_STATE_UNKNOWN
};

class Enemy:public Character
{
public:
	Animation_state anim_state;//saves the current animation state
	enemy_states state;
	enemy_states last_state;
	collider_type collider;
	p2DynArray<iPoint> path;
	int chase_distance;//variable set when creating the enemy, its the vision radius of the enemy
	int player_distance;//the distance to the player in number of tiles

	pugi::xml_document enemiesdoc;
	pugi::xml_node enemiesnode;
	p2SString filename;

public:
	Enemy(int x, int y, enemy_states startingstate=E_STATE_DEFAULT, EntityType atype = E_TYPE_ENEMY);
	virtual ~Enemy();
	virtual void Move();
	void Draw();
	// draws path and enemy collideres
	void Enemy::BlitEnemiesLogic();
	// Checks if the animation has to be changed
	virtual void CheckAnimation(enemy_states currentstate, enemy_states laststate);
	virtual void ChangeAnimation();
	int CheckDistance(float x, float y);
	int Enemy::CheckDistance2(int x, int y); // alternate method result of a branch merge
	void DoEnable();//enables the entity when in vision of the camera and disables it when not
	void DoPathFinding(); // fills the path with the result of the pathfinding
};

#endif // __ENEMY_H__